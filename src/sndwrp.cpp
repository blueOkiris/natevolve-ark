// API-level implementation of Soundwarp functionality

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <variant>
#include <sstream>
#include <string>
#include <err.hpp>
#include <sndwrp.hpp>

using namespace natevolve;
using namespace sndwrp;

Result<std::vector<SoundChange>> SoundChange::fromFile(const char *const fileName) {
    std::wifstream file(fileName);
    if (!file.is_open()) {
        return Error { ErrorType::FileOpen, "Failed to open '" + std::string(fileName) + "'" };
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

        SoundChange change;
        col = 1;
        while (col - 1 < line.length() && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
            col++;
        }

        // Get the first phoneme
        if (col - 1 >= line.length()) {
            return Error {
                ErrorType::FileFormat,
                "Expected phoneme at line " + std::to_string(ln) + ", col " + std::to_string(col)
            };
        }
        change.a = line[col - 1];
        col++;
        while (col - 1 < line.length() && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
            col++;
        }

        // Then the '>'
        if (col - 1 >= line.length() || line[col - 1] != L'>') {
            return Error {
                ErrorType::FileFormat,
                "Expected '>' at line " + std::to_string(ln) + ", col " + std::to_string(col)
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
                "Expected phoneme at line " + std::to_string(ln) + ", col " + std::to_string(col)
            };
        }
        change.b = line[col - 1];
        col++;
        while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
            col++;
        }

        // Get the '/'
        if (col - 1 >= line.length() || line[col - 1] != L'/') {
            return Error {
                ErrorType::FileFormat,
                "Expected '/' at line " + std::to_string(ln) + ", col " + std::to_string(col)
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
                "Expected '{' at line " + std::to_string(ln) + ", col " + std::to_string(col)
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
                "Expected '}' at line " + std::to_string(ln) + ", col " + std::to_string(col)
            };
        }
        while (line[col - 1] != L'}') {
            change.frntCond.push_back(line[col - 1]);
            col++;
            while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
                col++;
            }
            if (col - 1 >= line.length()) {
                return Error {
                    ErrorType::FileFormat,
                    "Expected '}' at line " + std::to_string(ln) + ", col " + std::to_string(col)
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
                "Expected '_' at line " + std::to_string(ln) + ", col " + std::to_string(col)
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
                "Expected '{' at line " + std::to_string(ln) + ", col " + std::to_string(col)
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
                "Expected '}' at line " + std::to_string(ln) + ", col " + std::to_string(col)
            };
        }
        while (line[col - 1] != L'}') {
            change.endCond.push_back(line[col - 1]);
            col++;
            while (col - 1 < line.length() && (line[col - 1] == L' ' || line[col - 1] == L'\t')) {
                col++;
            }
            if (col - 1 >= line.length()) {
                return Error {
                    ErrorType::FileFormat,
                    "Expected '}' at line " + std::to_string(ln) + ", col " + std::to_string(col)
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
                "Extra characters on line " + std::to_string(ln)
            };
        }

        changes.push_back(change);
        ln++;
    }

    if (file.bad()) {
        return Error {
            ErrorType::FileRead,
            "Failed to read at line " + std::to_string(ln) + ", col " + std::to_string(col)
        };
    }

    file.close();
    return changes;
}

//std::variant<std::vector<SoundChange>, Error>

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

