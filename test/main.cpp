// Examples of how to use the library

#include <variant>
#include <vector>
#include <iostream>
#include <sndwrp.hpp>

void print_changes(const std::vector<sndwrp::SoundChange> &changes);
bool test_apply(const std::vector<sndwrp::SoundChange> &changes);

int main(int argc, char **argv) {
    const auto changes = sndwrp::SoundChange::fromFile("test/test-changes.sw");
    if (std::holds_alternative<sndwrp::Error>(changes)) {
        std::cout
            << "Error loading sound changes from file." << std::endl
            << "Error ID: " << static_cast<int>(std::get<sndwrp::Error>(changes).type) << std::endl
            << "Error Message: " << std::get<sndwrp::Error>(changes).message << std::endl;
        return 1;
    }

    print_changes(std::get<std::vector<sndwrp::SoundChange>>(changes));
    if (!test_apply(std::get<std::vector<sndwrp::SoundChange>>(changes))) {
        return 1;
    }

    return 0;
};

void print_changes(const std::vector<sndwrp::SoundChange> &changes) {
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

bool test_apply(const std::vector<sndwrp::SoundChange> &changes) {
    const auto words = std::vector<std::wstring>({
        L"fak",
        L"faki",
        L"alphat",
        L"at",
        L"pxm"
    });

    for (const auto &word : words) {
        std::wcout << L"Applying changes to '" << word << "'" << std::endl;
        auto changedWord = word;
        for (size_t i = 0; i < changes.size(); i++) {
            std::wcout << i << ". /" << changedWord << L"/ → /";
            const auto change = changes[i];
            const auto newChangedWord = change.apply(changedWord);
            if (std::holds_alternative<sndwrp::Error>(newChangedWord)) {
                std::cout << std::endl;
                std::wcout
                    << L"Error applying sound change on word "
                        << word << L" on change " << i << std::endl
                    << L"Error ID: "
                        << static_cast<int>(std::get<sndwrp::Error>(newChangedWord).type)
                        << std::endl;
                std::cout << "Error Message: "
                        << std::get<sndwrp::Error>(newChangedWord).message
                        << std::endl;
                return 1;
            }
            changedWord = std::get<std::wstring>(newChangedWord);
            std::wcout << changedWord << L"/" << std::endl;
        }
    }
    return true;
}

