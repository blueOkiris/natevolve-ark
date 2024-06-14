// Implement wordup functionality

#include <random>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <optional>
#include <err.hpp>
#include <natevolve.hpp>
#include <wordup.hpp>

using namespace natevolve;
using namespace wordup;

enum class FileParseState {
    Categories,
    Vowels,
    Onsets,
    Codas,
    End
};

Result<Generator> Generator::fromFile(const char *const fileName) {
    std::wifstream file(fileName);
    if (!file.is_open()) {
        return Error {
            ErrorType::FileOpen,
            L"Failed to open '" + toWstr(std::string(fileName)) + L"'"
        };
    }

    std::map<std::wstring, std::vector<std::wstring>> categories;
    std::vector<std::wstring> vowels;
    std::vector<std::vector<std::wstring>> onsetOptions;
    std::vector<std::vector<std::wstring>> codaOptions;
    size_t ln = 1;
    size_t col = 1;
    std::wstring line;
    auto state = FileParseState::Categories;
    while (std::getline(file, line)) {
        if (line.empty()) {
            ln++;
            continue;
        }

        if (line == L"#") {
            state = static_cast<FileParseState>(static_cast<size_t>(state) + 1);
            if (state == FileParseState::End) {
                // We should reach EOF, not a final #
                return Error {
                    ErrorType::FileFormat,
                    L"Unexpected extra section at EOF in '" + toWstr(std::string(fileName)) + L"'"
                };
            }
            continue;
        }

        col = 1;
        while (col - 1 < line.length() && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
            col++;
        }

        switch (state) {
            case FileParseState::End:
                // Shouldn't happen
                return Error {
                    ErrorType::FileFormat,
                    L"Unexpected extra section at EOF in '" + toWstr(std::string(fileName)) + L"'"
                };

            case FileParseState::Categories: {
                // Get the category name
                if (col - 1 >= line.length()) {
                    return Error {
                        ErrorType::FileFormat,
                        L"Expected category name in '" + toWstr(std::string(fileName))
                            + L"' at line " + std::to_wstring(ln)
                            + L", col " + std::to_wstring(col)
                    };
                }
                std::wstringstream name;
                while (col - 1 < line.length()
                        && (line[col - 1] != L' ' && line[col - 1] != L'\t')) {
                    name << line[col - 1];
                    col++;
                }
                while (col - 1 < line.length()
                        && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
                    col++;
                }

                // Get the '{'
                if (col - 1 >= line.length() || line[col - 1] != L'{') {
                    return Error {
                        ErrorType::FileFormat,
                        L"Expected '{' in '" + toWstr(std::string(fileName))
                            + L"' at line " + std::to_wstring(ln)
                            + L", col " + std::to_wstring(col)
                    };
                }
                col++;
                while (col - 1 < line.length()
                        && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
                    col++;
                }

                // Get the onset character options
                std::vector<std::wstring> sounds;
                if (col - 1 >= line.length()) {
                    return Error {
                        ErrorType::FileFormat,
                        L"Expected '}' at line " + std::to_wstring(ln)
                            + L", col " + std::to_wstring(col)
                    };
                }
                while (line[col - 1] != L'}') {
                    // Get the sound
                    std::wstringstream sound;
                    while (col - 1 < line.length()
                            && (
                                line[col - 1] != L' ' && line[col - 1] != L'\t'
                                && line[col - 1] != L'}'
                            )) {
                        sound << line[col - 1];
                        col++;
                    }
                    while (col - 1 < line.length()
                            && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
                        col++;
                    }
                    sounds.push_back(sound.str());

                    if (col - 1 >= line.length()) {
                        return Error {
                            ErrorType::FileFormat,
                            L"Expected '}' in '" + toWstr(std::string(fileName))
                                + L"' at line " + std::to_wstring(ln)
                                + L", col " + std::to_wstring(col)
                        };
                    }
                }
                col++;
                while (col - 1 < line.length()
                        && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
                    col++;
                }

                if (col - 1 != line.length()) {
                    return Error {
                        ErrorType::FileFormat,
                        L"Extra characters in '" + toWstr(std::string(fileName))
                            + L"' on line " + std::to_wstring(ln)
                    };
                }

                categories.insert({ name.str(), sounds });
                break;
            }

            case FileParseState::Vowels:
                vowels.push_back(line);
                break;

            case FileParseState::Onsets: {
                std::vector<std::wstring> option;
                while (col - 1 < line.length()) {
                    std::wstringstream cat;
                    while (col - 1 < line.length()
                            && (line[col - 1] != L' ' && line[col - 1] != L'\t')) {
                        cat << line[col - 1];
                        col++;
                    }
                    while (col - 1 < line.length()
                            && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
                        col++;
                    }
                    option.push_back(cat.str());
                }
                onsetOptions.push_back(option);
                break;
            }

            case FileParseState::Codas: {
                std::vector<std::wstring> option;
                while (col - 1 < line.length()) {
                    std::wstringstream cat;
                    while (col - 1 < line.length()
                            && (line[col - 1] != L' ' && line[col - 1] != L'\t')) {
                        cat << line[col - 1];
                        col++;
                    }
                    while (col - 1 < line.length()
                            && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
                        col++;
                    }
                    option.push_back(cat.str());
                }
                codaOptions.push_back(option);
                break;
            }
        }

        ln++;
    }

    if (file.bad()) {
        return Error {
            ErrorType::FileRead,
            L"Failed to read '" + toWstr(std::string(fileName))
                + L"' at line " + std::to_wstring(ln) + L", col " + std::to_wstring(col)
        };
    }

    file.close();
    return Generator(categories, vowels, onsetOptions, codaOptions);
}

Generator::Generator(
    const std::map<std::wstring, std::vector<std::wstring>> &cats,
    const std::vector<std::wstring> &vwls,
    const std::vector<std::vector<std::wstring>> &onsets,
    const std::vector<std::vector<std::wstring>> &codas):
        categories(cats), vowels(vwls), onsetOptions(onsets), codaOptions(codas) {}


Result<std::wstring> Generator::generate(void) const {
    std::wstringstream word;

    // Generate a random onset
    std::uniform_int_distribution<size_t> onsetOptionsDist(0, onsetOptions.size() - 1);
    const auto onsetOption = onsetOptionsDist(g_rng);
    for (const auto &cat : onsetOptions[onsetOption]) {
        if (cat == L"∅") {
            // No onset
            break;
        }
        // Add a sound from every category in the randomly selected onset
        try {
            const auto sounds = categories.at(cat);
            std::uniform_int_distribution<size_t> soundDist(0, sounds.size() - 1);
            const auto sound = soundDist(g_rng);
            word << sounds[sound];
        } catch (std::out_of_range &) {
            return Error { ErrorType::UnknownCategory, cat };
        }
    }

    // Pick a random vowel
    std::uniform_int_distribution<size_t> vowelDist(0, vowels.size() - 1);
    const auto vowel = vowelDist(g_rng);
    word << vowels[vowel];

    // Generate a random coda
    std::uniform_int_distribution<size_t> codaOptionsDist(0, codaOptions.size() - 1);
    const auto codaOption = codaOptionsDist(g_rng);
    for (const auto &cat : codaOptions[codaOption]) {
        if (cat == L"∅") {
            // No coda
            break;
        }
        // Add a sound from every category in the randomly selected onset
        try {
            const auto sounds = categories.at(cat);
            std::uniform_int_distribution<size_t> soundDist(0, sounds.size() - 1);
            const auto sound = soundDist(g_rng);
            word << sounds[sound];
        } catch (std::out_of_range &) {
            return Error { ErrorType::UnknownCategory, cat };
        }
    }

    return word.str();
}

std::optional<Error> Generator::toFile(const char *const fileName) const {
    std::wofstream file(fileName);
    if (!file.is_open()) {
        return Error {
            ErrorType::FileOpen,
            L"Failed to open '" + toWstr(std::string(fileName)) + L"' for writing"
        };
    }

    for (const auto &cat : categories) {
        file << cat.first << L" { ";
        for (const auto &sound : cat.second) {
            file << sound << L" ";
        }
        file << L"}\n";
    }

    file << L"#\n";

    for (const auto &vowel : vowels) {
        file << vowel << "\n";
    }
    
    file << L"#\n";

    for (const auto &opt : onsetOptions) {
        for (const auto &cat : opt) {
            file << cat << L" ";
        }
        file << L'\n';
    }
    
    file << L"#\n";

    for (const auto &opt : codaOptions) {
        for (const auto &cat : opt) {
            file << cat << L" ";
        }
        file << L'\n';
    }

    file.close();
    return std::nullopt;
}

