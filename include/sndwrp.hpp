// API for Soundwarp module

#pragma once

#include <vector>
#include <variant>
#include <err.hpp>

namespace natevolve {
    namespace sndwrp {
        // Represents a mapping of a > b / X_X
        struct SoundChange {
            // -------- Functions --------

            // Load in a vector of sound changes from a .sw file
            //
            // File format is lines of the following syntax:
            // <sound> '>' <sound> '/' '{' { <sound> | '#' } '}' '_' '{' { <sound> | '#' } '}'
            // Ex: f>v/{#}_{}
            static Result<std::vector<SoundChange>> fromFile(const char *const fileName);

            SoundChange(
                const wchar_t ca, const wchar_t cb,
                const std::vector<wchar_t> &fCond, const std::vector<wchar_t> &eCond
            );

            // Given a word in IPA format, apply this sound change to it
            Result<std::wstring> apply(const std::wstring &word) const;

            // -------- Members ---------

            // The sound to identify
            const wchar_t a;

            // The sound to replace it with. For things like apocope, use ∅
            const wchar_t b;

            // The set of front sounds that can trigger this change.
            // Empty list ignores front condition while # represents word initial boundary
            // Something like all vowels would be { L'a', L'i', L'u' }
            // or whatever vowels exist in the lang
            const std::vector<wchar_t> frntCond;

            // Same thing as above but for post-context
            const std::vector<wchar_t> endCond;
        };

        // Given a set of changes, apply each one in order
        Result<std::wstring> applyAllChanges(
            const std::wstring &word,
            const std::vector<SoundChange> &changes
        );
    }
}

