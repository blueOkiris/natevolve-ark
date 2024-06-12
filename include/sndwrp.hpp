// API for sndwrp lib

#pragma once

#include <vector>
#include <variant>
#include <err.hpp>

namespace sndwrp {
    // Represents a mapping of a > b / X_X
    // File format is lines of the following syntax:
    // <sound> '>' <sound> '/' '{' { <sound> | '#' } '}' '_' '{' { <sound> | '#' } '}'
    // Ex: f>v/{#}_{}
    struct SoundChange {
        // The sound to identify
        wchar_t a;

        // The sound to replace it with. For things like apocope, use ∅
        wchar_t b;

        // The set of front sounds that can trigger this change.
        // Empty list ignores front condition while # represents word initial boundary
        // Something like all vowels would be { L'a', L'i', L'u' }
        // or whatever vowels exist in the lang
        std::vector<wchar_t> frntCond;

        // Same thing as above but for post-context
        std::vector<wchar_t> endCond;

        static std::variant<std::vector<SoundChange>, Error> fromFile(const char *const fileName);

        std::variant<std::wstring, Error> apply(const std::wstring &word) const;
    };
}

