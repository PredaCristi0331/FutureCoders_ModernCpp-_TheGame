
#include <crow.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "../Database/Database/DatabaseManager.h"
#include "GameSessionManager.h"
#include "MatchmakingQueue.h"

// Global instances
DatabaseManager dbManager("thegame.db");
GameSessionManager gameManager;
MatchmakingQueue matchmakingQueue;

// Helper function to hash password (simple version - in production use bcrypt/argon2)
std::string hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(password));
}

int main() {
    // Initialize database
    dbManager.init();

    // Create Crow app
    crow::SimpleApp app;

    // CORS middleware (for development)
    app.loglevel(crow::LogLevel::Info);

    // ============ AUTHENTICATION ENDPOINTS ============

    // POST /api/auth/register
    CROW_ROUTE(app, "/api/auth/register").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, R"({"success": false, "message": "Invalid JSON"})");
        }

        std::string username = body["username"].s();
        std::string password = body.has("password") ? std::string(body["password"].s()) : "";

        // Check if user exists
        auto existingUser = dbManager.getUserByName(username);
        if (existingUser) {
            return crow::response(400, R"({"success": false, "message": "Username already exists"})");
        }

        // Create user
        std::string passwordHash = password.empty() ? "" : hashPassword(password);
        int userId = dbManager.createUser(username, passwordHash);

        if (userId < 0) {
            return crow::response(500, R"({"success": false, "message": "Failed to create user"})");
        }

        crow::json::wvalue response;
        response["success"] = true;
        response["message"] = "User created successfully";
        response["userId"] = userId;
        response["username"] = username;

        return crow::response(200, response);
    });

    // POST /api/auth/login
    CROW_ROUTE(app, "/api/auth/login").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, R"({"success": false, "message": "Invalid JSON"})");
        }

        std::string username = body["username"].s();
        std::string password = body.has("password") ? std::string(body["password"].s()) : "";

        // Get user
        auto user = dbManager.getUserByName(username);
        if (!user) {
            // Auto-create user if doesn't exist (for simplicity)
            std::string passwordHash = password.empty() ? "" : hashPassword(password);
            int userId = dbManager.createUser(username, passwordHash);

            if (userId < 0) {
                return crow::response(500, R"({"success": false, "message": "Failed to create user"})");
            }

            crow::json::wvalue response;
            response["success"] = true;
            response["message"] = "User created and logged in";
            response["userId"] = userId;
            response["username"] = username;

            return crow::response(200, response);
        }

        // Verify password if provided
        if (!password.empty() && !user->passwordHash.empty()) {
            std::string passwordHash = hashPassword(password);
            if (passwordHash != user->passwordHash) {
                return crow::response(401, R"({"success": false, "message": "Invalid password"})");
            }
        }

        crow::json::wvalue response;
        response["success"] = true;
        response["message"] = "Login successful";
        response["userId"] = user->id;
        response["username"] = user->username;

        return crow::response(200, response);
    });

    // ============ MATCHMAKING ENDPOINTS ============

    // POST /api/matchmaking/join
    CROW_ROUTE(app, "/api/matchmaking/join").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, R"({"success": false, "message": "Invalid JSON"})");
        }

        int userId = body["userId"].i();
        std::string username = body["username"].s();

        // Get user profile to get performance score
        auto profile = dbManager.getUserProfile(userId);
        double performanceScore = profile ? profile->avgPerformance : 2.5; // Default mid-score

        // Add to matchmaking queue
        matchmakingQueue.addPlayer(userId, username, performanceScore);

        crow::json::wvalue response;
        response["success"] = true;
        response["message"] = "Joined matchmaking queue";
        response["queueSize"] = static_cast<int>(matchmakingQueue.getQueueSize());

        return crow::response(200, response);
    });

    // GET /api/matchmaking/status?userId=<id>
    CROW_ROUTE(app, "/api/matchmaking/status")
    ([](const crow::request& req) {
        auto userId = req.url_params.get("userId");
        if (!userId) {
            return crow::response(400, R"({"success": false, "message": "Missing userId"})");
        }

        int userIdInt = std::stoi(userId);

        // 1. Check if user is already in a game (created by another player's poll)
        int existingGameId = gameManager.findGameIdByPlayerId(userIdInt);
        if (existingGameId != -1) {
            crow::json::wvalue response;
            response["success"] = true;
            response["status"] = "matched";
            response["gameId"] = existingGameId;
            response["message"] = "Game found!";
            return crow::response(200, response);
        }

        // 2. Try to match players
        auto match = matchmakingQueue.tryMatch();

        if (!match.empty()) {
            // Check if this user is in the match
            bool userInMatch = false;
            for (const auto& player : match) {
                if (player.userId == userIdInt) {
                    userInMatch = true;
                    break;
                }
            }

            if (userInMatch) {
                // Create game session in database
                int dbGameId = dbManager.createGameSession(static_cast<int>(match.size()), 0); // 0 = normal difficulty

                // Extract player IDs and names
                std::vector<int> playerIds;
                std::vector<std::string> playerNames;
                for (const auto& player : match) {
                    playerIds.push_back(player.userId);
                    playerNames.push_back(player.username);
                }

                // Create game in GameSessionManager
                int gameId = gameManager.createGame(playerIds, playerNames, dbGameId);

                crow::json::wvalue response;
                response["success"] = true;
                response["status"] = "matched";
                response["gameId"] = gameId;
                response["message"] = "Game starting!";

                return crow::response(200, response);
            }
        }

        // Still waiting
        bool inQueue = matchmakingQueue.isPlayerInQueue(userIdInt);

        crow::json::wvalue response;
        response["success"] = true;
        response["status"] = inQueue ? "waiting" : "not_in_queue";
        response["queueSize"] = static_cast<int>(matchmakingQueue.getQueueSize());

        return crow::response(200, response);
    });

    // ============ GAME ENDPOINTS ============

    // GET /api/game/<gameId>/state?userId=<id>
    CROW_ROUTE(app, "/api/game/<int>/state")
    ([](const crow::request& req, int gameId) {
        auto userId = req.url_params.get("userId");
        if (!userId) {
            return crow::response(400, R"({"success": false, "message": "Missing userId"})");
        }

        int userIdInt = std::stoi(userId);

        if (!gameManager.gameExists(gameId)) {
            return crow::response(404, R"({"success": false, "message": "Game not found"})");
        }

        auto state = gameManager.getGameState(gameId, userIdInt);

        crow::json::wvalue response;
        response["success"] = true;
        std::string stateJson = state.dump();
        response["data"] = crow::json::load(stateJson);

        return crow::response(200, response);
    });

    // POST /api/game/<gameId>/play
    CROW_ROUTE(app, "/api/game/<int>/play").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req, int gameId) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, R"({"success": false, "message": "Invalid JSON"})");
        }

        int userId = body["userId"].i();
        int cardValue = body["cardValue"].i();
        int pileIndex = body["pileIndex"].i();

        if (!gameManager.gameExists(gameId)) {
            return crow::response(404, R"({"success": false, "message": "Game not found"})");
        }

        bool success = gameManager.playCard(gameId, userId, cardValue, pileIndex);

        if (!success) {
            return crow::response(400, R"({"success": false, "message": "Invalid move"})");
        }

        crow::json::wvalue response;
        response["success"] = true;
        response["message"] = "Card played successfully";

        return crow::response(200, response);
    });

    // POST /api/game/<gameId>/chat
    CROW_ROUTE(app, "/api/game/<int>/chat").methods(crow::HTTPMethod::Post)
    ([](const crow::request& req, int gameId) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, R"({"success": false, "message": "Invalid JSON"})");
        }

        std::string playerName = body["playerName"].s();
        std::string message = body["message"].s();

        if (!gameManager.gameExists(gameId)) {
            return crow::response(404, R"({"success": false, "message": "Game not found"})");
        }

        gameManager.sendChatMessage(gameId, playerName, message);

        crow::json::wvalue response;
        response["success"] = true;
        response["message"] = "Message sent";

        return crow::response(200, response);
    });

    // ============ USER PROFILE ENDPOINT ============

    // GET /api/user/<userId>/profile
    CROW_ROUTE(app, "/api/user/<int>/profile")
    ([](int userId) {
        auto profile = dbManager.getUserProfile(userId);

        if (!profile) {
            return crow::response(404, R"({"success": false, "message": "User not found"})");
        }

        crow::json::wvalue response;
        response["success"] = true;
        crow::json::wvalue data;
        data["userId"] = profile->userId;
        data["username"] = profile->username;
        data["totalGames"] = profile->totalGames;
        data["gamesWon"] = profile->gamesWon;
        data["winRate"] = profile->winRate;
        data["avgCardsLostGames"] = profile->avgCardsLostGames;
        data["avgPerformance"] = profile->avgPerformance;
        response["data"] = std::move(data);

        return crow::response(200, response);
    });

    // Start server
    std::cout << "========================================" << std::endl;
    std::cout << "   THE GAME - SERVER" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Server starting on http://localhost:18080" << std::endl;
    std::cout << "Database: thegame.db" << std::endl;
    std::cout << "========================================" << std::endl;

    app.port(18080).multithreaded().run();

    return 0;
}
