// Small, global helper functions for the whole library

#pragma once

#include <codecvt>

namespace natevolve {
    static inline void enableUtf8(void) {
        std::locale::global(std::locale(""));
#ifdef _WIN32
        _setmode(_fileno(stdout), _O_WTEXT);
#else
        std::setlocale(LC_ALL, "");
#endif
    }
};

