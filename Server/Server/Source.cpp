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

int main()
{
    crow::SimpleApp app;

    // Chat
    http::ChatStorage storage;
    storage.Initialize();
    http::ChatHeandler chat(storage);
    
    http::GameSessionManager gameManager;
    http::AuthHandler auth;

    // Auth endpoints
    CROW_ROUTE(app, "/auth/register").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        return auth.Register(req);
            });

    CROW_ROUTE(app, "/auth/login").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        return auth.Login(req);
            });

    CROW_ROUTE(app, "/auth/logout").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        return auth.Logout(req);
            });

    // Chat endpoints
    CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        return chat.PostMessage(req);
            });

    CROW_ROUTE(app, "/chat/<int>")
        ([&](int64_t gameId) {
        return chat.GetMessages(static_cast<int>(gameId));
            });

    // Game session endpoints
    CROW_ROUTE(app, "/game/create").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        return gameManager.CreateGame(req);
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

    app.port(18080).multithreaded().run();
}