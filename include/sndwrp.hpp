// API for Soundwarp module

#pragma once

#include <vector>
#include <variant>
#include <err.hpp>

namespace natevolve {
    namespace sndwrp {
        // Represents a mapping of a > b / X_X
        struct SoundChange {
            // -------- Public Members ---------

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

            // -------- Functions --------

            // Load in a vector of sound changes from a .sw file
            //
            // File format is lines of the following syntax:
            // <sound> '>' <sound> '/' '{' { <sound> | '#' } '}' '_' '{' { <sound> | '#' } '}'
            // Ex: f>v/{#}_{}
            static Result<std::vector<SoundChange>> fromFile(const char *const fileName);

            // Given a set of changes, apply each one in order
            //static Result<std::wstring> applyAll(const std::vector<SoundChange> &changes);

            // Given a word in IPA format, apply this sound change to it
            Result<std::wstring> apply(const std::wstring &word) const;
        };
    }
}

