// Small, global helper functions for the whole library

#pragma once
#ifdef _WIN32
#include <fcntl.h>
#endif
#include <locale>
#include <string>
#include <random>
#include <codecvt>

namespace natevolve {
    static std::random_device g_randDev;
    static std::mt19937 g_rng(g_randDev());

    static inline void enableUtf8(void) {
#ifdef _WIN32
        std::setlocale(LC_ALL, ".utf8");
        _setmode(_fileno(stdout), _O_WTEXT);
#else
        std::locale::global(std::locale(""));
        std::setlocale(LC_ALL, "");
#endif
    }

    static inline std::wstring toWstr(const std::string &src) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(src);
    }
};

