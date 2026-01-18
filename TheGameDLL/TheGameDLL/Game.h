#pragma once
#include "Deck.h"
#include "Pile.h"
#include "Player.h"
#include "Export.h"
#include <vector>
#include <memory>
#include <array>
#include <string>
#include <functional>

class THEGAME_API Game {
private:
    std::unique_ptr<Deck> deck_;
    std::array<Pile, 4> piles_;
    std::vector<std::unique_ptr<Player>> players_;
    bool gameOver_;

    
    std::function<void(const std::string&)> onGameEvent_;

public:
    Game();

    
    Game(Game&& other) noexcept = default;
    Game& operator=(Game&& other) noexcept = default;

    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void addPlayer(std::string name);

    void startGame();

    bool playCard(size_t playerIndex, size_t cardIndex, size_t pileIndex);

    [[nodiscard]] bool isGameOver() const noexcept { return gameOver_; }

    [[nodiscard]] bool checkWinCondition() const;

    [[nodiscard]] std::string getGameState() const;

    void setEventCallback(std::function<void(const std::string&)> callback) {
        onGameEvent_ = std::move(callback);
    }

    
    template<typename... Actions>
    void executeActions(Actions&&... actions) {
        (std::forward<Actions>(actions)(), ...);
    }

    
    [[nodiscard]] auto getActivePlayers() const {
        return players_
            | std::views::filter([](const auto& player) {
            return player->hasCards();
                })
            | std::views::transform([](const auto& player) {
            return player->getName();
                });
    }
};