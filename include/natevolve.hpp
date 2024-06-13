// Small, global helper functions for the whole library

#pragma once

#include <locale>
#include <string>
#include <random>
#include <codecvt>

namespace natevolve {
    static std::random_device g_randDev;
    static std::mt19937 g_rng(g_randDev());

    static inline void enableUtf8(void) {
        std::locale::global(std::locale(""));
#ifdef _WIN32
        _setmode(_fileno(stdout), _O_WTEXT);
#else
        std::setlocale(LC_ALL, "");
#endif
    }

    static inline std::wstring toWstr(const std::string &src) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(src);
    }
};

