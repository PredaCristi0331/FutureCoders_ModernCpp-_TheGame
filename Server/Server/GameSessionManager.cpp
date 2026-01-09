#include "GameSessionManager.h"
#include "RequestValidator.h"

namespace http
{
    crow::response GameSessionManager::CreateGame(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto validationResult = http::RequestValidator::ValidateJSON(req);
        if (validationResult)
        {
            return std::move(validationResult.errorResponse);
        }

        auto body = crow::json::load(req.body);
        int maxPlayers = 4;
        
        if (body.has("maxPlayers"))
        {
            maxPlayers = body["maxPlayers"].i();
            if (maxPlayers < 2 || maxPlayers > 8)
            {
                return http::RequestValidator::CreateErrorResponse(400, "Invalid maxPlayers",
                    "maxPlayers must be between 2 and 8");
            }
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
            return http::RequestValidator::CreateErrorResponse(404, "Game not found",
                "No game exists with ID " + std::to_string(gameId));
        }

        GameSession& session = it->second;

        if (session.currentPlayers >= session.maxPlayers)
        {
            return http::RequestValidator::CreateErrorResponse(400, "Game is full",
                "This game has reached maximum capacity");
        }

        auto validationResult = http::RequestValidator::ValidateJSON(req);
        if (validationResult)
        {
            return std::move(validationResult.errorResponse);
        }

        auto body = crow::json::load(req.body);
        std::string playerName = "Anonymous";
        
        if (body.has("playerName"))
        {
            playerName = body["playerName"].s();
            auto lengthValidation = http::RequestValidator::ValidateStringLength(playerName, "playerName", 1, 20);
            if (lengthValidation)
            {
                return std::move(lengthValidation.errorResponse);
            }
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
            return http::RequestValidator::CreateErrorResponse(404, "Game not found",
                "No game exists with ID " + std::to_string(gameId));
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
    crow::response GameSessionManager::StartGame(int gameId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end()) return crow::response(404, "Game not found");
        it->second.status = "playing";
        return crow::response(200, "Game started");
    }
    crow::response GameSessionManager::EndGame(int gameId)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_sessions.find(gameId);
        if (it == m_sessions.end()) return crow::response(404, "Game not found");
        it->second.status = "finished";
        return crow::response(200, "Game ended");
    }
}
