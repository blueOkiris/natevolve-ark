// Examples of how to use the library

#include <variant>
#include <vector>
#include <iostream>
#include <err.hpp>
#include <sndwrp.hpp>

void print_changes(const std::vector<natevolve::sndwrp::SoundChange> &changes);
bool test_apply(const std::vector<natevolve::sndwrp::SoundChange> &changes);

int main(int argc, char **argv) {
    const auto changes = natevolve::sndwrp::SoundChange::fromFile("test/test-changes.sw");
    if (natevolve::isErr(changes)) {
        std::cout
            << "Error loading sound changes from file." << std::endl
            << "Error ID: " << static_cast<int>(natevolve::err(changes).type) << std::endl
            << "Error Message: " << natevolve::err(changes).message << std::endl;
        return 1;
    }

    print_changes(natevolve::ok(changes));
    if (!test_apply(natevolve::ok(changes))) {
        return 1;
    }

    return 0;
};

void print_changes(const std::vector<natevolve::sndwrp::SoundChange> &changes) {
    for (const auto &change : changes) {
        std::cout << "Change:" << std::endl;
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

bool test_apply(const std::vector<natevolve::sndwrp::SoundChange> &changes) {
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
                std::cout << std::endl;
                std::wcout
                    << L"Error applying sound change on word "
                        << word << L" on change " << i << std::endl
                    << L"Error ID: "
                        << static_cast<int>(natevolve::err(newChangedWord).type)
                        << std::endl;
                std::cout << "Error Message: "
                        << natevolve::err(newChangedWord).message
                        << std::endl;
                return 1;
            }
            changedWord = natevolve::ok(newChangedWord);
            std::wcout << changedWord << L"/" << std::endl;
        }

        std::cout << "Expected: ";
        const auto result = natevolve::sndwrp::applyAllChanges(word, changes);
        if (natevolve::isErr(result)) {
            std::wcout << L"Error (" << static_cast<int>(natevolve::err(result).type) << L") - ";
            std::cout << natevolve::err(result).message << std::endl;
        } else {
            std::wcout << natevolve::ok(result) << std::endl;
        }
    }
    return true;
}

