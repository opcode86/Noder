#pragma once
#include <intrin.h>

#ifndef OBFUSCATOR
#define OBFUSCATOR
constexpr size_t key() {
    return(
        ~(__TIME__[0] * 0x9a1637) ^ 0x88c07e ^
        (__TIME__[1] * 0x8211ce) ^ 0xb636cd ^
        ~(__TIME__[3] * 0xb2f087) ^ 0x524bc8 ^
        (__TIME__[4] * 0x089d32) ^ 0x6f1e08 ^
        ~(__TIME__[6] * 0x69cdf0) ^ 0xe7cd3a ^
        (__TIME__[7] * 0x7d4954) ^ 0x23daff
        );
}

#define KEY key()

template <typename T>
__forceinline constexpr void encrypt(T* a, size_t size, size_t key) {
    for (size_t i = 0; i < size; i++)
        a[i] ^= (T)key;
}

template <typename T>
__forceinline constexpr void decrypt(T* a, size_t size, size_t key) {
    for (size_t i = 0; i < size; i++)
    {
        const __m128i xor_key_m128 = _mm_cvtsi32_si128(key);
        const __m128i val_m128 = _mm_cvtsi32_si128(a[i]);
        const __m128i xored_val_m128 = _mm_xor_epi32(val_m128, xor_key_m128);

        a[i] = _mm_cvtsi128_si32(xored_val_m128);
    }
};

template<typename T, size_t size, size_t key>
class Obf {
public:
    constexpr Obf(const T* str) {
        for (size_t i = 0; i < size; i++)
            data[i] = str[i];

        encrypt(data, size, key);
    }

    constexpr const T* getData() const {
        decrypt(const_cast<T*>(data), size, key);

        return &data[0];
    }

private:
    T data[size]{};
};

// Obfuscation macro
#define OBF(type, n) \
    []() -> const type* const { \
        constexpr auto O = Obf<type, sizeof(n)/sizeof(n[0]), KEY>(n); \
        return O.getData(); \
    }()
#endif