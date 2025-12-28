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
#include "RateLimiter.h"
#include "HealthCheck.h"
#include "RequestValidator.h"

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
    http::RateLimiter authLimiter(5, 5, 60);
    http::HealthCheck healthCheck("1.0.0");

    http::Logger::Log(http::Logger::Level::INFO, "All handlers initialized successfully");

    // Auth endpoints
    CROW_ROUTE(app, "/auth/register").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::Logger::LogRequest("POST", "/auth/register");
        
        std::string ip = req.remote_ip_address;
        if (!authLimiter.AllowRequest(ip))
        {
            http::Logger::Log(http::Logger::Level::WARNING, "Rate limit exceeded for IP: " + ip);
            return http::RequestValidator::CreateErrorResponse(429, "Too many requests", 
                "Rate limit exceeded. Please try again later.");
        }
        
        auto response = auth.Register(req);
        http::Logger::LogResponse(response.code, "/auth/register");
        return response;
            });

    CROW_ROUTE(app, "/auth/login").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::Logger::LogRequest("POST", "/auth/login");
        
        std::string ip = req.remote_ip_address;
        if (!authLimiter.AllowRequest(ip))
        {
            http::Logger::Log(http::Logger::Level::WARNING, "Rate limit exceeded for IP: " + ip);
            return http::RequestValidator::CreateErrorResponse(429, "Too many requests",
                "Rate limit exceeded. Please try again later.");
        }
        
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

    // Health check endpoint
    CROW_ROUTE(app, "/health")
        ([&]() -> crow::response {
        return healthCheck.GetHealth();
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