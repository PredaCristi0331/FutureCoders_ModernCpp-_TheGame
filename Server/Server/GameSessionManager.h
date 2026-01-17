#pragma once
#include <crow.h>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <memory>

// Forward declaration for C++ Module integration
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
        std::string status; // "waiting", "playing", "finished"
        std::vector<std::string> playerNames;
        std::vector<int> playerIds; // Map index to database ID, if needed
        
        // Game Logic State
        std::shared_ptr<game::GameTable> table;
        int currentPlayerIndex = 0; // 0 to maxPlayers-1
        int cardsPlayedThisTurn = 0; // Track cards played in current turn
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

        // Gameplay methods
        crow::response PlayCard(int gameId, const crow::request& req);
        crow::response EndTurn(int gameId, const crow::request& req); // New method
        crow::response GetGameState(int gameId, int userId); // userId to hide other hands
    };
}
