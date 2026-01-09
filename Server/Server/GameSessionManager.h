#pragma once
#include <crow.h>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace http
{
    struct GameSession
    {
        int gameId;
        int maxPlayers;
        int currentPlayers;
        std::string status; // "waiting", "playing", "finished"
        std::vector<std::string> playerNames;
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
    };
}
