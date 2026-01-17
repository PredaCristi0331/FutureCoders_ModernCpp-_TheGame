#pragma once
#include <crow.h>

namespace http
{
    class ChatHeandler
    {
    public:
        ChatHeandler();

        crow::response PostMessage(const crow::request& req);
        crow::response GetMessages(int gameId);
    };
}