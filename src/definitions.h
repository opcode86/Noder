#pragma once
#include <windows.h>

//
// Structs
//
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	WCHAR* Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _ANSI_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PCHAR  Buffer;
} ANSI_STRING, * PANSI_STRING;

typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;


//
// Functions
// 
using f_LdrLoadDll = NTSTATUS(__fastcall*)(
	_In_opt_ PWSTR Path,
	_In_opt_ PULONG LoadFlags,
	_In_ PUNICODE_STRING Name,
	_Out_ HMODULE* BaseAddress
	);

using f_LdrUnloadDll = NTSTATUS(__fastcall*)(
	_In_ PVOID BaseAddress
	);

using f_LdrGetProcedureAddressForCaller = NTSTATUS(__fastcall*)(
	_In_ HMODULE DllHandle,
	_In_opt_ PANSI_STRING ProcedureName,
	_In_opt_ ULONG ProcedureNumber, //maybe WORD
	_Out_ PVOID* ProcedureAddress,
	_In_ ULONG Flags, //maybe flags
	_In_ PVOID* Callback
	);

using f_RtlInitUnicodeString = NTSTATUS(__fastcall*)(
	_Out_ PUNICODE_STRING DestinationString,
	_In_opt_ PWSTR SourceString
	);

using f_RtlInitAnsiString = NTSTATUS(__fastcall*)(
	_Out_ PANSI_STRING DestinationString,
	_In_opt_ PCSTR SourceString
	);

using f_URLDownloadToFileW = HRESULT(__fastcall*)(
	_In_opt_	LPUNKNOWN pCaller,
	_In_		LPCWSTR szURL,
	_In_opt_	LPCWSTR szFileName,
	_Reserved_	DWORD dwReserved,
	_In_opt_	LPBINDSTATUSCALLBACK lpfnCB
	);

using f_NtDeleteFile = NTSTATUS(__fastcall*)(
	_In_ POBJECT_ATTRIBUTES ObjectAttributes
	);