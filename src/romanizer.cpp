// Implementation of Romanizer funcitonality

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <codecvt>
#include <iostream>
#include <err.hpp>
#include <natevolve.hpp>
#include <romanizer.hpp>

using namespace natevolve;
using namespace romanizer;

Result<Romanizer> Romanizer::fromFile(const char *const fileName) {
    std::wifstream file(fileName);
    if (!file.is_open()) {
        return Error {
            ErrorType::FileOpen,
            toWstr("Failed to open '" + std::string(fileName) + "'")
        };
    }

    std::map<wchar_t, std::wstring> ipaToRom;
    std::map<std::wstring, wchar_t> romToIpa;
    size_t ln = 1;
    size_t col = 1;
    std::wstring line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            ln++;
            continue;
        }

        std::wcout << line << std::endl;

        col = 1;
        while (col - 1 < line.length() && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
            col++;
        }

        // Get the phoneme
        if (col - 1 >= line.length()) {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected phoneme in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        wchar_t a = line[col - 1];
        col++;
        while (col - 1 < line.length() && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
            col++;
        }
        std::wcout << a << std::endl;

        // Get the romanization character(s)
        if (col - 1 >= line.length()) {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Expected romanization string in '" + std::string(fileName)
                        + "' at line " + std::to_string(ln) + ", col " + std::to_string(col)
                )
            };
        }
        std::wstringstream b;
        while (col - 1 < line.length() && (line[col - 1] != L' ' && line[col - 1] != L'\t')) {
            b << line[col - 1];
            col++;
        }
        while (col - 1 < line.length() && (line[col - 1] == ' ' || line[col - 1] == '\t')) {
            col++;
        }

        if (col - 1 != line.length()) {
            return Error {
                ErrorType::FileFormat,
                toWstr(
                    "Extra characters in '" + std::string(fileName) + "' on line "
                        + std::to_string(ln) + " at col " + std::to_string(col)
                )
            };
        }

        ipaToRom.insert({ a, b.str() });
        romToIpa.insert({ b.str(), a });
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
    return Romanizer(ipaToRom, romToIpa);
}

Romanizer::Romanizer(
    const std::map<wchar_t, std::wstring> &ipaToRom,
    const std::map<std::wstring, wchar_t> &romToIpa):
        ipaToRomanization(ipaToRom), romanizationToIpa(romToIpa) {}

std::wstring Romanizer::romanize(const std::wstring &ipaWord) const {
    std::wstringstream romWord;
    for (const auto c : ipaWord) {
        try {
            romWord << ipaToRomanization.at(c);
        } catch (std::out_of_range &) {
            romWord << c;
        }
    }
    return romWord.str();
}

std::wstring Romanizer::unromanize(const std::wstring &romWord) const {
    std::wstringstream ipaWord;
    for (size_t i = 0; i < romWord.length(); i++) {
        bool matched = false;
        for (const auto &romMap : romanizationToIpa) {
            if (romWord.substr(i).length() < romMap.first.length()) {
                continue;
            }
            if (romWord.substr(i, romMap.first.length()) == romMap.first) {
                ipaWord << romMap.second;
                i += romMap.first.length() - 1;
                matched = true;
                break;
            }
        }
        if (!matched) {
            ipaWord << romWord[i];
        }
    }
    return ipaWord.str();
}

