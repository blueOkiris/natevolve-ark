// API for interacting with Romanizer module

#pragma once

#include <map>
#include <string>
#include <err.hpp>

namespace natevolve {
    namespace romanizer {
        struct Romanizer {
            // -------- Functions --------

            // Read in a mapping of IPA to Romanization from a file
            // Format:
            // - Each line is two characters, an IPA symbol and a Romanization symbol
            // - You do not have to put pairs that match, e.g. if /p/ maps to <p>, you can ignore
            static Result<Romanizer> fromFile(const char *const fileName);

            Romanizer(
                const std::map<wchar_t, std::wstring> &ipaToRom,
                const std::map<std::wstring, wchar_t> &romToIpa
            );

            // Convert a word written with IPA symbols into the loaded Romanization.
            // Unknown symbols are left alone as an assumption they are 1:1 between Rom. and IPA
            std::wstring romanize(const std::wstring &ipaWord) const;

            // Convert a word written in the Romanization to its IPA pronunciation.
            // Unknown symbols are left alone as an assumption they are 1:1 between Rom. and IPA
            std::wstring unromanize(const std::wstring &romWord) const;

            // -------- Members --------

            // What special IPA symbols are mapped to what characters in a Romanization
            const std::map<wchar_t, std::wstring> ipaToRomanization;

            // The reverse of above to simplify code and speed up conversions
            const std::map<std::wstring, wchar_t> romanizationToIpa;
        };
    }
}

