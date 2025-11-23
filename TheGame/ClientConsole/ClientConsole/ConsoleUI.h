#pragma once
#include "GameState.h"
#include <string>

class ConsoleUI {
public:
    void Clear();
    void ShowWelcome();
    std::string PromptInput(const std::string& prompt);
    void ShowLobby(bool isWaiting);
    void ShowGame(const GameState& state);
    void ShowMessage(const std::string& msg);
    void ShowError(const std::string& err);
};
