#include "GameSessionManager.h"

namespace http
{
    crow::response GameSessionManager::CreateGame(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto body = crow::json::load(req.body);
        int maxPlayers = 4;
        if (body.has("maxPlayers")) {
            maxPlayers = body["maxPlayers"].i();
        }

        GameSession session;
        session.gameId = m_nextGameId++;
        session.maxPlayers = maxPlayers;
        session.currentPlayers = 0;
        session.status = "waiting";

        m_sessions[session.gameId] = session;

        crow::json::wvalue response;
        response["gameId"] = session.gameId;
        response["status"] = "created";
        response["maxPlayers"] = maxPlayers;

        return crow::response(200, response);
    }

    crow::response GameSessionManager::JoinGame(int gameId, const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end())
        {
            return crow::response(404, "Game not found");
        }

        GameSession& session = it->second;

        if (session.currentPlayers >= session.maxPlayers)
        {
            return crow::response(400, "Game is full");
        }

        auto body = crow::json::load(req.body);
        std::string playerName = "Anonymous";
        if (body.has("playerName")) {
            playerName = body["playerName"].s();
        }

        session.playerNames.push_back(playerName);
        session.currentPlayers++;

        if (session.currentPlayers >= 2 && session.currentPlayers == session.maxPlayers)
        {
            session.status = "playing";
        }

        crow::json::wvalue response;
        response["gameId"] = gameId;
        response["playerName"] = playerName;
        response["currentPlayers"] = session.currentPlayers;
        response["status"] = session.status;

        return crow::response(200, response);
    }

    crow::response GameSessionManager::GetGameInfo(int gameId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end())
        {
            return crow::response(404, "Game not found");
        }

        const GameSession& session = it->second;

        crow::json::wvalue response;
        response["gameId"] = session.gameId;
        response["maxPlayers"] = session.maxPlayers;
        response["currentPlayers"] = session.currentPlayers;
        response["status"] = session.status;
        
        std::vector<crow::json::wvalue> players;
        for (const auto& name : session.playerNames) {
            players.push_back(crow::json::wvalue(name));
        }
        response["players"] = std::move(players);

        return crow::response(200, response);
    }

    crow::response GameSessionManager::GetAllGames()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        crow::json::wvalue games;
        games = crow::json::wvalue::list();
        
        int idx = 0;
        for (const auto& [id, session] : m_sessions)
        {
            if (session.status == "waiting")
            {
                crow::json::wvalue game;
                game["gameId"] = session.gameId;
                game["currentPlayers"] = session.currentPlayers;
                game["maxPlayers"] = session.maxPlayers;
                game["status"] = session.status;
                games[idx++] = std::move(game);
            }
        }

        return crow::response(200, games);
    }
}
