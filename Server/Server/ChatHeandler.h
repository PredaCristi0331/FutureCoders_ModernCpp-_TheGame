#pragma once
#include <crow.h>
#include "ChatStorage.h"

namespace http
{
    class ChatHeandler
    {
    public:
        ChatHeandler(ChatStorage& storage);

        crow::response PostMessage(const crow::request& req);
        crow::response GetMessages(int gameId);

    private:
        ChatStorage& m_storage;
    };
}