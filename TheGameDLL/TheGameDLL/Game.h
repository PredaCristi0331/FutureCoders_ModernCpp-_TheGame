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

    // Lambda pentru callbacks
    std::function<void(const std::string&)> onGameEvent_;

public:
    Game();

    // Move semantics
    Game(Game&& other) noexcept = default;
    Game& operator=(Game&& other) noexcept = default;

    // Delete copy
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

    // Variadic template pentru acțiuni multiple
    template<typename... Actions>
    void executeActions(Actions&&... actions) {
        (std::forward<Actions>(actions)(), ...);
    }

    // Ranges pentru interogări
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