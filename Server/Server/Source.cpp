#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <filesystem>

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
#include "../Database/Database/DatabaseManager.h"
#include "ChatHeandler.h"
#include "GameSessionManager.h"
#include "AuthHandler.h"
#include "Logger.h"
#include "ServerStats.h"
#include "RateLimiter.h"
#include "HealthCheck.h"
#include "RequestValidator.h"
#include "ApiVersion.h"
#include "RequestTimer.h"
#include "CorsMiddleware.h"

int main()
{
    try {
        crow::SimpleApp app;

    app.loglevel(crow::LogLevel::Info);

    http::Logger::Log(http::Logger::Level::SUCCESS, "Server starting on port 18080...");

    // Initialize Database
    DatabaseManager::init("game.db");
    http::Logger::Log(http::Logger::Level::INFO, "Database initialized at: " + std::filesystem::absolute("game.db").string());

    // Chat
    http::ChatHeandler chat;
    
    http::GameSessionManager gameManager;
    http::AuthHandler auth;
    http::StatsManager stats;
    http::RateLimiter authLimiter(5, 5, 60);
    http::HealthCheck healthCheck("1.0.0");
    http::ApiVersion apiVersion("1.0.0", "FutureCoders Game API");

    http::Logger::Log(http::Logger::Level::INFO, "All handlers initialized successfully");

    // CORS preflight endpoint
    CROW_ROUTE(app, "/<path>").methods(crow::HTTPMethod::OPTIONS)
        ([&](std::string path) -> crow::response {
        return http::CorsMiddleware::HandlePreflight();
            });

    // Auth endpoints
    CROW_ROUTE(app, "/auth/register").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::RequestTimer timer("/auth/register");
        http::Logger::LogRequest("POST", "/auth/register");
        
        std::string ip = req.remote_ip_address;
        if (!authLimiter.AllowRequest(ip))
        {
            http::Logger::Log(http::Logger::Level::WARNING, "Rate limit exceeded for IP: " + ip);
            return http::RequestValidator::CreateErrorResponse(429, "Too many requests", 
                "Rate limit exceeded. Please try again later.");
        }
        
        auto response = auth.Register(req);
        http::CorsMiddleware::AddCorsHeaders(response);
        http::Logger::LogResponse(response.code, "/auth/register");
        return response;
            });

    CROW_ROUTE(app, "/auth/login").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::RequestTimer timer("/auth/login");
        http::Logger::LogRequest("POST", "/auth/login");
        
        std::string ip = req.remote_ip_address;
        if (!authLimiter.AllowRequest(ip))
        {
            http::Logger::Log(http::Logger::Level::WARNING, "Rate limit exceeded for IP: " + ip);
            return http::RequestValidator::CreateErrorResponse(429, "Too many requests",
                "Rate limit exceeded. Please try again later.");
        }
        
        auto response = auth.Login(req);
        http::CorsMiddleware::AddCorsHeaders(response);
        http::Logger::LogResponse(response.code, "/auth/login");
        return response;
            });

    CROW_ROUTE(app, "/auth/logout").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::RequestTimer timer("/auth/logout");
        http::Logger::LogRequest("POST", "/auth/logout");
        auto response = auth.Logout(req);
        http::CorsMiddleware::AddCorsHeaders(response);
        http::Logger::LogResponse(response.code, "/auth/logout");
        return response;
            });

    // Chat endpoints
    CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        http::RequestTimer timer("/chat");
        http::Logger::LogRequest("POST", "/chat");
        auto response = chat.PostMessage(req);
        http::CorsMiddleware::AddCorsHeaders(response);
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
        http::RequestTimer timer("/game/create");
        http::Logger::LogRequest("POST", "/game/create");
        auto response = gameManager.CreateGame(req);
        http::CorsMiddleware::AddCorsHeaders(response);
        http::Logger::LogResponse(response.code, "/game/create");
        return response;
            });


    // Join game endpoint
    CROW_ROUTE(app, "/game/<int>/join").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req, int gameId) {
        http::RequestTimer timer("/game/join");
        http::Logger::LogRequest("POST", "/game/" + std::to_string(gameId) + "/join");
        
        auto response = gameManager.JoinGame(gameId, req);
        http::CorsMiddleware::AddCorsHeaders(response);
        http::Logger::LogResponse(response.code, "/game/" + std::to_string(gameId) + "/join");
        return response;
            });

    CROW_ROUTE(app, "/game/<int>")
        ([&](int64_t gameId) -> crow::response {
        return gameManager.GetGameInfo(static_cast<int>(gameId));
            });

    CROW_ROUTE(app, "/game/<int>/play").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req, int gameId) {
        http::Logger::LogRequest("POST", "/game/" + std::to_string(gameId) + "/play");
        auto response = gameManager.PlayCard(gameId, req);
        http::CorsMiddleware::AddCorsHeaders(response);
        return response;
            });
            
    CROW_ROUTE(app, "/game/<int>/endturn").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req, int gameId) {
        http::Logger::LogRequest("POST", "/game/" + std::to_string(gameId) + "/endturn");
        auto response = gameManager.EndTurn(gameId, req);
        http::CorsMiddleware::AddCorsHeaders(response);
        return response;
            });

    // Profile Endpoint
    CROW_ROUTE(app, "/user/<int>/profile")
        ([&](int userId) {
            
            
            // 2. Get Profile
            auto profile = DatabaseManager::getUserProfile(userId);
            
            crow::json::wvalue json;
            json["username"] = profile.username;
            json["games_played"] = profile.games_played;
            json["games_won"] = profile.games_won;
            json["games_lost"] = profile.games_lost;
            json["performance_score"] = profile.performance_score;
            json["hours_played_seconds"] = profile.hours_played_seconds;
            
            crow::response response(200, json);
            http::CorsMiddleware::AddCorsHeaders(response);
            return response;
        });

    CROW_ROUTE(app, "/game/<int>/state")
        ([&](const crow::request& req, int gameId) -> crow::response {
         // Get userId from query param for now, e.g. ?userId=0
         // In real app, from Token/Auth
         char* userParam = req.url_params.get("userId");
         int userId = userParam ? std::stoi(userParam) : -1;
         
        return gameManager.GetGameState(static_cast<int>(gameId), userId);
            });

    CROW_ROUTE(app, "/game/<int>/start").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req, int gameId) {
        return gameManager.StartGame(gameId);
            });

    CROW_ROUTE(app, "/game/<int>/debug/win").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req, int gameId) {
        return gameManager.ForceWin(gameId);
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
        http::RequestTimer timer("/stats");
        http::Logger::LogRequest("GET", "/stats");
        auto response = stats.GetStats();
        http::CorsMiddleware::AddCorsHeaders(response);
        http::Logger::LogResponse(response.code, "/stats");
        return response;
            });

    // API version endpoint
    CROW_ROUTE(app, "/api/version")
        ([&]() -> crow::response {
        http::RequestTimer timer("/api/version");
        http::Logger::LogRequest("GET", "/api/version");
        auto response = apiVersion.GetVersion();
        http::CorsMiddleware::AddCorsHeaders(response);
        http::Logger::LogResponse(response.code, "/api/version");
        return response;
            });

        app.port(18080).multithreaded().run();
    }
    catch (const std::system_error& e) {
        std::cerr << "System Error: " << e.what() << " Code: " << e.code() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown Error occurred." << std::endl;
        return 1;
    }
    return 0;
}