#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <memory>
#include <crow/json.h>

// Forward declarations
struct ActiveGameSession;

using json = crow::json::wvalue;

class GameSessionManager {
public:
    GameSessionManager();
    ~GameSessionManager();

    int createGame(const std::vector<int>& playerIds, const std::vector<std::string>& playerNames, int dbGameId);
    json getGameState(int gameId, int playerId);
    bool playCard(int gameId, int playerId, int cardValue, int pileIndex);
    void sendChatMessage(int gameId, const std::string& playerName, const std::string& message);
    bool gameExists(int gameId) const;
    ActiveGameSession* getGame(int gameId);
    void endGame(int gameId, bool teamWon);
    bool isPlayerTurn(int gameId, int playerId);
    int findGameIdByPlayerId(int playerId);

private:
    std::unordered_map<int, std::unique_ptr<ActiveGameSession>> m_games;
    std::mutex m_mutex;
    int m_nextGameId;
};
