// API for Wordup generator

#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <err.hpp>

namespace natevolve {
    namespace wordup {
        struct Generator {
            // -------- Functions --------

            static Result<Generator> fromFile(const char *const fileName);

            Generator(
                const std::map<std::wstring, std::vector<std::wstring>> &cats,
                const std::vector<std::wstring> &vwls,
                const std::vector<std::vector<std::wstring>> &onsets,
                const std::vector<std::vector<std::wstring>> &codas
            );

            // Given a set up Generator, create a word for me
            Result<std::wstring> generate(void) const;

            // Store the generator settings in a file
            std::optional<Error> toFile(const char *const fileName) const;

            // -------- Members --------

            // A mapping from a character to the list of symbols it represents.
            // I.e. C for consonant might map to p, t, k, d͡ʒ, etc
            const std::map<std::wstring, std::vector<std::wstring>> categories;

            // Allowed vowels in the language
            const std::vector<std::wstring> vowels;

            // What are allowed onsets? { C }, { C, C }, { C, L }, etc
            const std::vector<std::vector<std::wstring>> onsetOptions;
            
            // What are allowed codas? { C }, { C, C }, { C, L }, etc
            const std::vector<std::vector<std::wstring>> codaOptions;
        };
    }
}

