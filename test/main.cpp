// Examples of how to use the library

#include <variant>
#include <vector>
#include <iostream>
#include <err.hpp>
#include <natevolve.hpp>
#include <sndwrp.hpp>
#include <romanizer.hpp>
#include <wordup.hpp>

void printChanges(const std::vector<natevolve::sndwrp::SoundChange> &changes);
bool testApply(const std::vector<natevolve::sndwrp::SoundChange> &changes);
void testRomanize(const natevolve::romanizer::Romanizer &romanizer);
void printGenData(const natevolve::wordup::Generator &gen);
void testWordGeneration(const natevolve::wordup::Generator &gen);

int main(int argc, char **argv) {
    natevolve::enableUtf8();

    const auto changes = natevolve::sndwrp::SoundChange::fromFile("test/test-changes.sw");
    if (natevolve::isErr(changes)) {
        std::wcout
            << L"Error loading sound changes from file." << std::endl
            << L"Error ID: " << static_cast<int>(natevolve::err(changes).type) << std::endl
            << L"Error Message: " << natevolve::err(changes).message << std::endl;
        return 1;
    }
    const auto romanizer = natevolve::romanizer::Romanizer::fromFile("test/test-romanization.rmz");
    if (natevolve::isErr(romanizer)) {
        std::wcout
            << L"Error loading romanization from file." << std::endl
            << L"Error ID: " << static_cast<int>(natevolve::err(romanizer).type) << std::endl
            << L"Error Message: " << natevolve::err(romanizer).message << std::endl;
        return 1;
    }
    const auto wordgen = natevolve::wordup::Generator::fromFile("test/test-wordgen.wu");
    if (natevolve::isErr(wordgen)) {
        std::wcout
            << L"Error loading Wordup Generator from file." << std::endl
            << L"Error ID: " << static_cast<int>(natevolve::err(wordgen).type) << std::endl
            << L"Error Message: " << natevolve::err(wordgen).message << std::endl;
        return 1;
    }

    printChanges(natevolve::ok(changes));
    if (!testApply(natevolve::ok(changes))) {
        return 1;
    }
    testRomanize(natevolve::ok(romanizer));
    printGenData(natevolve::ok(wordgen));
    testWordGeneration(natevolve::ok(wordgen));

    return 0;
};

void printChanges(const std::vector<natevolve::sndwrp::SoundChange> &changes) {
    for (const auto &change : changes) {
        std::wcout << L"Change:" << std::endl;
        std::wcout << L"- " << change.a << std::endl;
        std::wcout << L"- " << change.b << std::endl;
        std::wcout << L"- { ";
        for (const auto &c : change.frntCond) {
            std::wcout << c << L' ';
        }
        std::wcout << L"}" << std::endl;
        std::wcout << L"- { ";
        for (const auto &c : change.endCond) {
            std::wcout << c << L' ';
        }
        std::wcout << L"}" << std::endl;
    }
}

bool testApply(const std::vector<natevolve::sndwrp::SoundChange> &changes) {
    const auto words = std::vector<std::wstring>({
        L"fak",
        L"faki",
        L"alphat",
        L"fat",
        L"pxm"
    });

    for (const auto &word : words) {
        std::wcout << L"Applying changes to '" << word << "'" << std::endl;
        auto changedWord = word;
        for (size_t i = 0; i < changes.size(); i++) {
            std::wcout << i << ". /" << changedWord << L"/ → /";
            const auto change = changes[i];
            const auto newChangedWord = change.apply(changedWord);
            if (natevolve::isErr(newChangedWord)) {
                std::wcout
                    << L"Error applying sound change on word "
                        << word << L" on change " << i << std::endl
                    << L"Error ID: "
                        << static_cast<int>(natevolve::err(newChangedWord).type)
                        << std::endl
                    << L"Error Message: "
                        << natevolve::err(newChangedWord).message
                        << std::endl;
                return false;
            }
            changedWord = natevolve::ok(newChangedWord);
            std::wcout << changedWord << L"/" << std::endl;
        }

        std::wcout << L"Expected: ";
        const auto result = natevolve::sndwrp::applyAllChanges(word, changes);
        if (natevolve::isErr(result)) {
            std::wcout
                << L"Error (" << static_cast<int>(natevolve::err(result).type)
                << L") - " << natevolve::err(result).message << std::endl;
        } else {
            std::wcout << natevolve::ok(result) << std::endl;
        }
    }
    return true;
}

void testRomanize(const natevolve::romanizer::Romanizer &romanizer) {
    const std::wstring ipaTestWord = L"ʃæθɑih";
    const std::wstring romTestWord = L"shathoihéllo";

    const auto genRomWord = romanizer.romanize(ipaTestWord);
    const auto genIpaWord = romanizer.unromanize(romTestWord);
    std::wcout
        << L"Romanizing '" << ipaTestWord << L"'. Expected: '" << romTestWord
        << L"'. Received: '" << genRomWord << "'" << std::endl
        << L"Unromanizing '" << romTestWord << "'. Expected: '" << ipaTestWord
        << L"'. Received: '" << genIpaWord << "'" << std::endl
        << L"Success? "
        << ((genRomWord == romTestWord) && (genIpaWord == ipaTestWord)) << std::endl;
}

void printGenData(const natevolve::wordup::Generator &gen) {
    for (const auto &cat : gen.categories) {
        std::wcout << L"Category:" << std::endl << L"- Name: " << cat.first << std::endl;
        for (const auto &sound : cat.second) {
            std::wcout << L"- Sound: " << sound << std::endl;
        }
    }

    for (const auto &vowel : gen.vowels) {
        std::wcout << L"Vowel: " << vowel << std::endl;
    }

    for (const auto &opt : gen.onsetOptions) {
        std::wcout << L"Onset Option:" << std::endl;
        for (const auto &cat : opt) {
            std::wcout << L"- Category: " << cat << std::endl;
        }
    }

    for (const auto &opt : gen.codaOptions) {
        std::wcout << L"Coda Option:" << std::endl;
        for (const auto &cat : opt) {
            std::wcout << L"- Category: " << cat << std::endl;
        }
    }
}

void testWordGeneration(const natevolve::wordup::Generator &gen) {
    for (int i = 0; i < 10; i++) {
        const auto newWord = gen.generate();
        if (natevolve::isErr(newWord)) {
            std::wcout
                << L"Error occurred while generating word" << std::endl
                << L"Error Id: " << static_cast<int>(natevolve::err(newWord).type) << std::endl
                << L"Error Message: " << natevolve::err(newWord).message << std::endl;
        } else {
            std::wcout << natevolve::ok(newWord) << std::endl;
        }
    }
}

