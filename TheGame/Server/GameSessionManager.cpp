#include "GameSessionManager.h"
#include <iostream>
#include <algorithm>

import GameTable;
import Card;
using namespace game;

struct ActiveGameSession {
    int gameId;
    int dbGameId;
    GameTable gameTable;
    std::vector<int> playerIds;
    std::vector<std::string> playerNames;
    int currentTurnIndex;
    std::vector<std::string> chatMessages;
    bool gameEnded;
    bool teamWon;
};

GameSessionManager::GameSessionManager() : m_nextGameId(1) {}

GameSessionManager::~GameSessionManager() = default;

int GameSessionManager::createGame(const std::vector<int>& playerIds, const std::vector<std::string>& playerNames, int dbGameId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    int gameId = m_nextGameId++;
    
    auto session = std::make_unique<ActiveGameSession>();
    session->gameId = gameId;
    session->dbGameId = dbGameId;
    // Initialize GameTable with number of players
    session->gameTable = GameTable(static_cast<int>(playerIds.size()));
    session->gameTable.AddGamer();
    session->gameTable.AddInitialCards();
    session->gameTable.MixingDeckCards();
    session->gameTable.IssuerCard();
    
    session->playerIds = playerIds;
    session->playerNames = playerNames;
    session->currentTurnIndex = 0;
    session->gameEnded = false;
    session->teamWon = false;
    
    m_games[gameId] = std::move(session);
    return gameId;
}

bool GameSessionManager::gameExists(int gameId) const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(m_mutex));
    return m_games.count(gameId) > 0;
}

ActiveGameSession* GameSessionManager::getGame(int gameId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_games.find(gameId);
    if (it != m_games.end()) {
        return it->second.get();
    }
    return nullptr;
}

json GameSessionManager::getGameState(int gameId, int playerId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_games.find(gameId);
    if (it == m_games.end()) {
        return json{ {"error", "Game not found"} };
    }

    auto& gameSession = *(it->second);
    auto playerIt = std::find(gameSession.playerIds.begin(), gameSession.playerIds.end(), playerId);
    if (playerIt == gameSession.playerIds.end()) {
        return json{ {"error", "Player not in game"} };
    }

    int playerIndex = static_cast<int>(std::distance(gameSession.playerIds.begin(), playerIt));

    json state;
    state["gameId"] = gameId;
    state["isMyTurn"] = (gameSession.currentTurnIndex == playerIndex);
    state["gameEnded"] = gameSession.gameEnded;
    state["teamWon"] = gameSession.teamWon;
    state["deckSize"] = gameSession.gameTable.SizeDeckCards();

    std::vector<crow::json::wvalue> piles;
    piles.push_back({{"isAscending", true}, {"topCardValue", gameSession.gameTable.GetLastCardFromIncreasingFirst().GetCardNumber()}});
    piles.push_back({{"isAscending", true}, {"topCardValue", gameSession.gameTable.GetLastCardFromIncreasingSecond().GetCardNumber()}});
    piles.push_back({{"isAscending", false}, {"topCardValue", gameSession.gameTable.GetLastCardFromDecreasingFirst().GetCardNumber()}});
    piles.push_back({{"isAscending", false}, {"topCardValue", gameSession.gameTable.GetLastCardFromDecreasingSecond().GetCardNumber()}});
    state["piles"] = std::move(piles);

    auto playerCards = gameSession.gameTable.GetCardsGamer(playerIndex);
    std::vector<crow::json::wvalue> hand;
    for (auto& card : playerCards) {
        hand.push_back({ {"value", card.GetCardNumber()} });
    }
    state["hand"] = std::move(hand);

    std::vector<crow::json::wvalue> otherPlayers;
    for (size_t i = 0; i < gameSession.playerIds.size(); ++i) {
        if (static_cast<int>(i) != playerIndex) {
            auto cards = gameSession.gameTable.GetCardsGamer(static_cast<int>(i));
            otherPlayers.push_back({{"id", gameSession.playerIds[i]}, {"name", gameSession.playerNames[i]}, {"cardCount", cards.size()}});
        }
    }
    state["otherPlayers"] = std::move(otherPlayers);
    state["chatMessages"] = gameSession.chatMessages;

    return state;
}

// Helper for playCard
static bool isValidCardPlay(GameTable& table, int cardValue, int pileIndex) {
    int topCard = 0;
    bool isAscending = true;

    switch (pileIndex) {
    case 0: topCard = table.GetLastCardFromIncreasingFirst().GetCardNumber(); isAscending = true; break;
    case 1: topCard = table.GetLastCardFromIncreasingSecond().GetCardNumber(); isAscending = true; break;
    case 2: topCard = table.GetLastCardFromDecreasingFirst().GetCardNumber(); isAscending = false; break;
    case 3: topCard = table.GetLastCardFromDecreasingSecond().GetCardNumber(); isAscending = false; break;
    default: return false;
    }

    if (isAscending) {
        return (cardValue > topCard) || (cardValue == topCard - 10);
    }
    else {
        return (cardValue < topCard) || (cardValue == topCard + 10);
    }
}

bool GameSessionManager::playCard(int gameId, int playerId, int cardValue, int pileIndex) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_games.find(gameId);
    if (it == m_games.end()) return false;

    auto& gameSession = *(it->second);
    
    // Check turn
    if (gameSession.playerIds[gameSession.currentTurnIndex] != playerId) return false;

    // Try to play card
    if (isValidCardPlay(gameSession.gameTable, cardValue, pileIndex)) {
        // Remove card from player hand
        Card card(cardValue);
        gameSession.gameTable.RemoveCardFromHand(gameSession.currentTurnIndex, card); 
        
        // Place card on pile
        switch (pileIndex) {
        case 0: gameSession.gameTable.PushIncreasingFirst(card); break;
        case 1: gameSession.gameTable.PushIncreasingSecond(card); break;
        case 2: gameSession.gameTable.PushDecreasingFirst(card); break;
        case 3: gameSession.gameTable.PushDecreasingSecond(card); break;
        }

        // Check win condition
        if (gameSession.gameTable.IsGameWon()) {
            gameSession.gameEnded = true;
            gameSession.teamWon = true;
        }
        
        // Pass turn logic (simplified)
        gameSession.currentTurnIndex = (gameSession.currentTurnIndex + 1) % gameSession.playerIds.size();
        
        return true;
    }

    return false;
}

void GameSessionManager::sendChatMessage(int gameId, const std::string& playerName, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_games.find(gameId);
    if (it != m_games.end()) {
        it->second->chatMessages.push_back(playerName + ": " + message);
        if (it->second->chatMessages.size() > 50) {
            it->second->chatMessages.erase(it->second->chatMessages.begin());
        }
    }
}

void GameSessionManager::endGame(int gameId, bool teamWon) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_games.find(gameId);
    if (it != m_games.end()) {
        it->second->gameEnded = true;
        it->second->teamWon = teamWon;
    }
}

bool GameSessionManager::isPlayerTurn(int gameId, int playerId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_games.find(gameId);
    if (it != m_games.end()) {
        return it->second->playerIds[it->second->currentTurnIndex] == playerId;
    }
    return false;
}

int GameSessionManager::findGameIdByPlayerId(int playerId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& pair : m_games) {
        const auto& gameSession = *(pair.second);
        if (!gameSession.gameEnded) {
            for (int id : gameSession.playerIds) {
                if (id == playerId) {
                    return gameSession.gameId;
                }
            }
        }
    }
    return -1;
}
