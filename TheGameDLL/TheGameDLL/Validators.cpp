#include "Validators.h"
#include<array>
#include<string>
#include<regex>



const std::regex playerNameRegex_(R"(^[a-zA-Z0-9_]{3,20}$)");
const std::regex playCommandRegex_(R"(^play\s+(\d+)\s+(\d+)$)");
const std::regex fileNameRegex_(R"(^[a-zA-Z0-9_-]+\.save$)");

bool Validators::validatePlayerName(const std::string& name) {
    return std::regex_match(name, playerNameRegex_);
}

std::optional<std::array<int, 2>> Validators::parsePlayCommand(const std::string& command) {
    std::smatch match;
    if (std::regex_match(command, match, playCommandRegex_)) {
        return std::array<int, 2>{std::stoi(match[1]), std::stoi(match[2])};
    }
    return std::nullopt;
}

bool Validators::isValidCardValue(int value) {
    return value >= 2 && value <= 99;
}

bool Validators::validateFileName(const std::string& filename) {
    return std::regex_match(filename, fileNameRegex_);
}