#pragma once
#include <windows.h>
#include <winhttp.h>

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

using f_NtDeleteFile = NTSTATUS(__fastcall*)(
	_In_ POBJECT_ATTRIBUTES ObjectAttributes
	);

using f_WinHttpOpen = HINTERNET(__fastcall*)(
	_In_opt_		LPCWSTR pszAgentW,
	_In_           DWORD   dwAccessType,
	_In_           LPCWSTR pszProxyW,
	_In_           LPCWSTR pszProxyBypassW,
	_In_           DWORD   dwFlags
	);

using f_WinHttpConnect = HINTERNET(__fastcall*)(
	_In_ HINTERNET     hSession,
	_In_ LPCWSTR       pswzServerName,
	_In_ INTERNET_PORT nServerPort,
	_In_ DWORD         dwReserved
	);

using f_WinHttpOpenRequest = HINTERNET(__fastcall*)(
	_In_ HINTERNET hConnect,
	_In_ LPCWSTR   pwszVerb,
	_In_ LPCWSTR   pwszObjectName,
	_In_ LPCWSTR   pwszVersion,
	_In_ LPCWSTR   pwszReferrer,
	_In_ LPCWSTR* ppwszAcceptTypes,
	_In_ DWORD     dwFlags
	);

using f_WinHttpSendRequest = BOOL(__fastcall*)(
	_In_          HINTERNET hRequest,
	_In_opt_ LPCWSTR   lpszHeaders,
	_In_           DWORD     dwHeadersLength,
	_In_opt_ LPVOID    lpOptional,
	_In_           DWORD     dwOptionalLength,
	_In_           DWORD     dwTotalLength,
	_In_           DWORD_PTR dwContext
	);

using f_WinHttpReceiveResponse = BOOL(__fastcall*)(
	_In_ HINTERNET hRequest,
	_In_ LPVOID    lpReserved
	);

using f_WinHttpQueryDataAvailable = BOOL(__fastcall*)(
	_In_  HINTERNET hRequest,
	_Out_ LPDWORD   lpdwNumberOfBytesAvailable
	);

using f_WinHttpReadData = BOOL(__fastcall*)(
	_In_  HINTERNET hRequest,
	_Out_ LPVOID    lpBuffer,
	_In_  DWORD     dwNumberOfBytesToRead,
	_Out_ LPDWORD   lpdwNumberOfBytesRead
	);

using f_WinHttpCloseHandle = BOOL(__fastcall*)(
	_In_ HINTERNET hInternet
	);
