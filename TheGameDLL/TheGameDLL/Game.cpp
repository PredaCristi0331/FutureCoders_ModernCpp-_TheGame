#include "Game.h"
#include "Logger.h"
#include <format>
#include <algorithm>
#include <ranges>

Game::Game()
    : deck_(std::make_unique<Deck>())
    , piles_{ Pile(PileType::Ascending), Pile(PileType::Ascending),
             Pile(PileType::Descending), Pile(PileType::Descending) }
    , gameOver_(false) {

    Logger::getInstance().info("Game initialized");
}

void Game::addPlayer(std::string name) {
    players_.push_back(std::make_unique<Player>(std::move(name)));
    Logger::getInstance().info("Player added:  {}", players_.back()->getName());
}

void Game::startGame() {
    
    for (auto& player : players_) {
        for (int i = 0; i < 7; ++i) {
            if (auto card = deck_->drawCard()) {
                player->addCard(std::move(*card));
            }
        }
    }

    Logger::getInstance().info("Game started with {} players", players_.size());

    if (onGameEvent_) {
        onGameEvent_("Game started!");
    }
}

bool Game::playCard(size_t playerIndex, size_t cardIndex, size_t pileIndex) {
    if (playerIndex >= players_.size() || pileIndex >= piles_.size()) {
        Logger::getInstance().warning("Invalid player or pile index");
        return false;
    }

    auto& player = players_[playerIndex];
    auto& pile = piles_[pileIndex];

    auto cardOpt = player->playCard(cardIndex);
    if (!cardOpt) {
        Logger::getInstance().warning("Invalid card index");
        return false;
    }

    auto cardPtr = std::shared_ptr<Card>(std::move(*cardOpt));

    if (!pile.placeCard(cardPtr)) {
        
        Logger::getInstance().warning("Cannot place card on pile");
        return false;
    }

    
    if (auto newCard = deck_->drawCard()) {
        player->addCard(std::move(*newCard));
    }

    Logger::getInstance().info("Player {} played card {} on pile {}",
        player->getName(), cardPtr->getValue(), pileIndex);

    if (onGameEvent_) {
        onGameEvent_(std::format("Player {} played card {}",
            player->getName(), cardPtr->getValue()));
    }

    return true;
}

bool Game::checkWinCondition() const {
   
    bool deckEmpty = deck_->isEmpty();
    bool allPlayersEmpty = std::ranges::all_of(players_,
        [](const auto& player) { return !player->hasCards(); });

    return deckEmpty && allPlayersEmpty;
}

std::string Game::getGameState() const {
    std::string state = "=== Game State ===\n";

   
    for (size_t i = 0; i < piles_.size(); ++i) {
        state += std::format("Pile {}: {} ({})\n",
            i,
            piles_[i].getTopValue().value_or(0),
            piles_[i].getType() == PileType::Ascending ? "ASC" : "DESC");
    }

    
    state += std::format("\nDeck: {} cards remaining\n", deck_->size());

    
    state += "\nPlayers:\n";
    for (const auto& player : players_) {
        state += std::format("  {}: {} cards\n",
            player->getName(),
            player->getHandSize());
    }

    return state;
}