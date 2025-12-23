#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <format>
#include"../TheGameDLL/Game.h"
#include"../TheGameDLL/Validators.h"
#include"../TheGameDLL/Logger.h"

// Template function pentru afișare container
template<typename Container>
void printContainer(const Container& container, std::string_view name) {
    std::cout << name << ": ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

// Variadic template pentru meniu
template<typename... MenuItems>
void displayMenu(MenuItems... items) {
    std::cout << "\n=== MENU ===\n";
    int index = 1;
    ((std::cout << index++ << ". " << items << "\n"), ...);
    std::cout << "============\n";
}

void demonstrateRanges(const Game& game) {
    std::cout << "\n=== Active Players (using Ranges) ===\n";
    for (const auto& playerName : game.getActivePlayers()) {
        std::cout << "  - " << playerName << "\n";
    }
}

void gameEventCallback(const std::string& event) {
    std::cout << "[EVENT] " << event << "\n";
}

int main() {
    std::cout << "=== The Game - C++20 Implementation ===\n\n";

    // Smart pointer demonstration
    auto game = std::make_unique<Game>();

    // Logger setup (Singleton)
    Logger::getInstance().setMinLevel(LogLevel::Info);
    Logger::getInstance().info("Application started");

    // Regex validation pentru nume jucători
    std::vector<std::string> playerNames;

    std::cout << "Enter number of players (1-4): ";
    int numPlayers;
    std::cin >> numPlayers;
    std::cin.ignore();

    for (int i = 0; i < numPlayers; ++i) {
        std::string name;
        bool valid = false;

        while (!valid) {
            std::cout << std::format("Enter name for Player {} (3-20 alphanumeric): ", i + 1);
            std::getline(std::cin, name);

            if (Validators::validatePlayerName(name)) {
                playerNames.push_back(name);
                game->addPlayer(name);
                valid = true;
            }
            else {
                std::cout << "Invalid name!  Use 3-20 alphanumeric characters.\n";
            }
        }
    }

    // Lambda pentru event handling
    game->setEventCallback([](const std::string& event) {
        std::cout << "[GAME EVENT] " << event << "\n";
        Logger::getInstance().info("Event: {}", event);
        });

    // Start game
    game->startGame();

    // Variadic template menu
    bool running = true;
    while (running && !game->isGameOver()) {
        displayMenu(
            "View Game State",
            "Play Card",
            "Show Active Players (Ranges Demo)",
            "Execute Multiple Actions",
            "Exit"
        );

        std::cout << "Choose option: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            // const ref demonstration
            const auto& gameState = game->getGameState();
            std::cout << "\n" << gameState << "\n";
            break;
        }

        case 2: {
            std::cout << "Enter command (e.g., 'play 0 1' for card 0 to pile 1): ";
            std::string command;
            std::getline(std::cin, command);

            // Regex parsing
            if (auto indices = Validators::parsePlayCommand(command)) {
                size_t playerIdx = 0; // Simplified for demo
                size_t cardIdx = (*indices)[0];
                size_t pileIdx = (*indices)[1];

                if (game->playCard(playerIdx, cardIdx, pileIdx)) {
                    std::cout << "Card played successfully!\n";
                }
                else {
                    std::cout << "Invalid move!\n";
                }
            }
            else {
                std::cout << "Invalid command format!\n";
            }
            break;
        }

        case 3: {
            // Ranges demonstration
            demonstrateRanges(*game);
            break;
        }

        case 4: {
            // Variadic template demonstration
            std::cout << "\nExecuting multiple actions...\n";
            game->executeActions(
                []() { std::cout << "Action 1: Checking game state\n"; },
                []() { std::cout << "Action 2: Logging status\n"; },
                []() { Logger::getInstance().info("Multiple actions executed"); }
            );
            break;
        }

        case 5: {
            running = false;
            break;
        }

        default:
            std::cout << "Invalid option!\n";
        }

        // Check win condition using modern algorithms
        if (game->checkWinCondition()) {
            std::cout << "\n🎉 CONGRATULATIONS! You won!  🎉\n";
            Logger::getInstance().info("Game won!");
            break;
        }
    }

    // Move semantics demonstration
    auto finalGame = std::move(game);
    std::cout << "\n=== Final Game State ===\n";
    std::cout << finalGame->getGameState() << "\n";

    Logger::getInstance().info("Application ended");

    std::cout << "\nCheck 'thegame.log' for detailed logs.\n";

    return 0;
}