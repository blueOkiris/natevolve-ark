// API-level implementation of Soundwarp functionality

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <sstream>
#include <string>
#include <codecvt>
#include <err.hpp>
#include <natevolve.hpp>
#include <sndwrp.hpp>

using namespace natevolve;
using namespace sndwrp;

SoundChange::SoundChange(
    const wchar_t ca, const wchar_t cb,
    const std::vector<wchar_t> &fCond, const std::vector<wchar_t> &eCond):
        a(ca), b(cb), frntCond(fCond), endCond(eCond) {}

Result<std::vector<SoundChange>> SoundChange::fromFile(const char *const fileName) {
    std::wifstream file(fileName);
    if (!file.is_open()) {
        return Error {
            ErrorType::FileOpen,
            toWstr("Failed to open '" + std::string(fileName) + "'")
        };
    }

    std::vector<SoundChange> changes;
    size_t ln = 1;
    size_t col = 1;
    std::wstring line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            ln++;
            continue;
        }

        wchar_t a = L'\0';
        wchar_t b = L'\0';
        std::vector<wchar_t> frntCond;
        std::vector<wchar_t> endCond;

        col = 1;
        while (col - 1 < line.length() && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
            col++;
        }

        // Get the first phoneme
        if (col - 1 >= line.length()) {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected phoneme in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        a = line[col - 1];
        col++;
        while (col - 1 < line.length() && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
            col++;
        }

        // Then the '>'
        if (col - 1 >= line.length() || line[col - 1] != L'>') {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected '>' in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        // Get the second phoneme
        if (col - 1 >= line.length()) {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected phoneme in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        b = line[col - 1];
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        // Get the '/'
        if (col - 1 >= line.length() || line[col - 1] != L'/') {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected '/' in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        // Get the {
        if (col - 1 >= line.length() || line[col - 1] != L'{') {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected '{' in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        // Get the front condition up to '}'
        if (col - 1 >= line.length()) {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected '}' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        while (line[col - 1] != L'}') {
            frntCond.push_back(line[col - 1]);
            col++;
            while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
                col++;
            }
            if (col - 1 >= line.length()) {
                return Error {
                    ErrorType::FileFormat,
                    toWstr(
                        "Expected '}' in '" + std::string(fileName)
                            + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                    )
                };
            }
        }
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        // Get the '_'
        if (col - 1 >= line.length() || line[col - 1] != L'_') {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected '_' in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        // Get the second {
        if (col - 1 >= line.length() || line[col - 1] != L'{') {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected '{' in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        // Get the end condition
        if (col - 1 >= line.length()) {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected '}' in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        while (line[col - 1] != L'}') {
            endCond.push_back(line[col - 1]);
            col++;
            while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
                col++;
            }
            if (col - 1 >= line.length()) {
                return Error {
                    ErrorType::FileFormat,
                    toWstr(
                        "Expected '}' in '" + std::string(fileName)
                            + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                    )
                };
            }
        }
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        if (col - 1 != line.length()) {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Extra characters in '" + std::string(fileName) + "' on line "
                        + std::to_string(ln)
                )
            };
        }

        changes.push_back(SoundChange(a, b, frntCond, endCond));
        ln++;
    }

    if (file.bad()) {
        return Error {
            ErrorType::FileRead,
            toWstr(
                "Failed to read '" + std::string(fileName)
                    + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
            )
        };
    }

    file.close();
    return changes;
}

Result<std::wstring> SoundChange::apply(const std::wstring &word) const {
    std::wstringstream changedWord;
    for (size_t i = 0; i < word.length(); i++) {
        const bool correctSound = word[i] == a;
        const bool frontCondFulfilled =
            (i == 0 && std::count(frntCond.begin(), frntCond.end(), L'#') > 0)
                || (i > 0 && std::count(frntCond.begin(), frntCond.end(), word[i - 1]) > 0)
                || frntCond.empty();
        const bool endCondFulfilled =
            (i == word.length() - 1 && std::count(endCond.begin(), endCond.end(), L'#') > 0)
                || (
                    i + 1 < word.length()
                        && std::count(endCond.begin(), endCond.end(), word[i + 1]) > 0
                ) || endCond.empty();
        const auto c = (correctSound && frontCondFulfilled && endCondFulfilled) ? b : word[i];
        changedWord << c;
    }
    return changedWord.str();
}

Result<std::wstring> natevolve::sndwrp::applyAllChanges(
        const std::wstring &word, const std::vector<SoundChange> &changes) {
    auto changedWord = word;
    for (const auto &change : changes) {
        const auto result = change.apply(changedWord);
        if (isErr(result)) {
            return err(result);
        }
        changedWord = ok(result);
    }
    return changedWord;
}

