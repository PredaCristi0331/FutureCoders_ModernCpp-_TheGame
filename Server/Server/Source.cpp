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

int main()
{
    crow::SimpleApp app;

    http::ChatStorage storage;
    storage.Initialize();

    http::ChatHeandler chat(storage);

    // POST /chat
    CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::POST)
        ([&](const crow::request& req) {
        return chat.PostMessage(req);
            });

    // GET /chat/<gameId>
    CROW_ROUTE(app, "/chat/<int>")
        ([&](int gameId) {
        return chat.GetMessages(gameId);
            });

    app.port(18080).multithreaded().run();
}