#pragma once
#include "Export.h"
#include <string>
#include <regex>
#include <optional>

class THEGAME_API Validators {
public:
    // Regex pentru validare nume jucãtor (alfanumeric, 3-20 caractere)
    static bool validatePlayerName(const std::string& name);

    // Regex pentru comenzi de joc
    static std::optional<std::array<int, 2>> parsePlayCommand(const std::string& command);

    // Validare card value
    static bool isValidCardValue(int value);

    // Regex pentru salvare/incarcare joc
    static bool validateFileName(const std::string& filename);

private:
    static const std::regex playerNameRegex_;
    static const std::regex playCommandRegex_;
    static const std::regex fileNameRegex_;
};