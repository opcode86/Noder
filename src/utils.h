#pragma once
#include <stdlib.h>

namespace utils
{
	__forceinline wchar_t* RandomString() noexcept
	{
		wchar_t retVal[32];

		for (int i = 0; i < 31; ++i)
		{
			unsigned int temp;

			do {
				temp = rand();
			} while (
				(temp > 90 || temp < 65) && // Capital letters
				(temp > 57 || temp < 48) && // Lower case letters
				(temp > 122 || temp < 97)   // Numbers
				);

			retVal[i] = (wchar_t)temp;
		}

		retVal[31] = L'\0';

		return (retVal);
	}
}