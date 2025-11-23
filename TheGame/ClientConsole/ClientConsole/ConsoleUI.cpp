#include "ConsoleUI.h"
#include <iostream>
#include <iomanip>

void ConsoleUI::Clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleUI::ShowWelcome() {
    Clear();
    std::cout << "========================================" << std::endl;
    std::cout << "           THE GAME - CLIENT            " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Register" << std::endl;
    std::cout << "3. Exit" << std::endl;
}

std::string ConsoleUI::PromptInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::cin >> input;
    return input;
}

void ConsoleUI::ShowLobby(bool isWaiting) {
    Clear();
    std::cout << "================ LOBBY =================" << std::endl;
    if (isWaiting) {
        std::cout << "Waiting for other players..." << std::endl;
        std::cout << "(Polling server...)" << std::endl;
    } else {
        std::cout << "1. Create/Join Game" << std::endl;
        std::cout << "2. Logout" << std::endl;
    }
}

void ConsoleUI::ShowGame(const GameState& state) {
    Clear();
    std::cout << "Game ID: " << state.gameId << " | Deck: " << state.deckSize << " cards" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    // Show Piles
    std::cout << "PILES:" << std::endl;
    for (size_t i = 0; i < state.piles.size(); ++i) {
        std::string type = state.piles[i].isAscending ? "^ ASC (1->99)" : "v DESC (100->2)";
        std::cout << "[" << i << "] " << type << ": " << state.piles[i].topCardValue << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    // Show Other Players
    std::cout << "PLAYERS:" << std::endl;
    for (const auto& p : state.otherPlayers) {
        std::cout << p.name << ": " << p.cardCount << " cards" << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    // Show Hand
    std::cout << "YOUR HAND:" << std::endl;
    for (const auto& card : state.hand) {
        std::cout << "[" << card.value << "] ";
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // Show Chat
    std::cout << "CHAT:" << std::endl;
    for (const auto& msg : state.chatMessages) {
        std::cout << msg << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
    
    if (state.isMyTurn) {
        std::cout << ">> YOUR TURN! <<" << std::endl;
    } else {
        std::cout << "Waiting for turn..." << std::endl;
    }
}

void ConsoleUI::ShowMessage(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;
}

void ConsoleUI::ShowError(const std::string& err) {
    std::cerr << "[ERROR] " << err << std::endl;
}
