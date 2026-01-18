#pragma once
#include <crow.h>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <memory>

namespace game {
    class GameTable;
}

namespace http
{
    struct GameSession
    {
        int gameId;
        int maxPlayers;
        int currentPlayers;
        std::string status;
        bool won = false;
        std::vector<std::string> playerNames;
        std::time_t startTime = 0;
        std::vector<int> playerIds;
        
        std::shared_ptr<game::GameTable> table;
        int currentPlayerIndex = 0;
        int cardsPlayedThisTurn = 0;
    };

    class GameSessionManager
    {
    private:
        std::map<int, GameSession> m_sessions;
        std::mutex m_mutex;
        int m_nextGameId = 1;

    public:
        crow::response CreateGame(const crow::request& req);
        crow::response JoinGame(int gameId, const crow::request& req);
        crow::response GetGameInfo(int gameId);
        crow::response GetAllGames();
        crow::response StartGame(int gameId);
        crow::response EndGame(int gameId);
        crow::response ForceWin(int gameId);

        crow::response PlayCard(int gameId, const crow::request& req);
        crow::response EndTurn(int gameId, const crow::request& req);
        crow::response GetGameState(int gameId, int userId);
    };
}
