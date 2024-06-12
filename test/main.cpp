// Examples of how to use the library

#include <variant>
#include <vector>
#include <iostream>
#include <sndwrp.hpp>

void print_changes(const std::vector<sndwrp::SoundChange> &changes);

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

