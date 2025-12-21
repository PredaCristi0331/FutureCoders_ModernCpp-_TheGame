#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <iostream>
import TheGame;

//int main() {
//	using namespace game;
//	TheGame gaming;
//	gaming.StartTheGame();
//	gaming.Rund();
//	return 0;
//}

#include <crow.h>
#include "ChatStorage.h"
#include "ChatHeandler.h"
#include "GameSessionManager.h"
#include "AuthHandler.h"
#include "Logger.h"
#include "ServerStats.h"

int main()
{
    crow::SimpleApp app;

    app.loglevel(crow::LogLevel::Info);

    http::Logger::Log(http::Logger::Level::SUCCESS, "Server starting on port 18080...");

    // Chat
    http::ChatStorage storage;
    storage.Initialize();
    http::ChatHeandler chat(storage);
    
    http::GameSessionManager gameManager;
    http::AuthHandler auth;
    http::StatsManager stats;

    http::Logger::Log(http::Logger::Level::INFO, "All handlers initialized successfully");

    // Auth endpoints
    CROW_ROUTE(app, "/auth/register").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::Logger::LogRequest("POST", "/auth/register");
        auto response = auth.Register(req);
        http::Logger::LogResponse(response.code, "/auth/register");
        return response;
            });

    CROW_ROUTE(app, "/auth/login").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::Logger::LogRequest("POST", "/auth/login");
        auto response = auth.Login(req);
        http::Logger::LogResponse(response.code, "/auth/login");
        return response;
            });

    CROW_ROUTE(app, "/auth/logout").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::Logger::LogRequest("POST", "/auth/logout");
        auto response = auth.Logout(req);
        http::Logger::LogResponse(response.code, "/auth/logout");
        return response;
            });

    // Chat endpoints
    CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::Logger::LogRequest("POST", "/chat");
        auto response = chat.PostMessage(req);
        http::Logger::LogResponse(response.code, "/chat");
        return response;
            });

    CROW_ROUTE(app, "/chat/<int>")
        ([&](int64_t gameId) {
        return chat.GetMessages(static_cast<int>(gameId));
            });

    // Game session endpoints
    CROW_ROUTE(app, "/game/create").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::Logger::LogRequest("POST", "/game/create");
        auto response = gameManager.CreateGame(req);
        http::Logger::LogResponse(response.code, "/game/create");
        return response;
            });


    /* Temporarily disabled - CROW parameter matching issue
    CROW_ROUTE(app, "/game/<int>/join").methods(crow::HTTPMethod::POST)
        ([&](int64_t gameId, const crow::request& req) -> crow::response {
        return gameManager.JoinGame(static_cast<int>(gameId), req);
            });
    */

    CROW_ROUTE(app, "/game/<int>")
        ([&](int64_t gameId) -> crow::response {
        return gameManager.GetGameInfo(static_cast<int>(gameId));
            });

    CROW_ROUTE(app, "/games")
        ([&]() -> crow::response {
        return gameManager.GetAllGames();
            });

    // Server stats endpoint
    CROW_ROUTE(app, "/stats")
        ([&]() -> crow::response {
        http::Logger::LogRequest("GET", "/stats");
        auto response = stats.GetStats();
        http::Logger::LogResponse(response.code, "/stats");
        return response;
            });

    app.port(18080).multithreaded().run();
}