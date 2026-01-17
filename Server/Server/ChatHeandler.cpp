#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "ChatHeandler.h"
#include "../Database/Database/DatabaseManager.h"
#include <ctime>

namespace http
{
    ChatHeandler::ChatHeandler()
    {
    }

    crow::response ChatHeandler::PostMessage(const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        int gameId = body["gameId"].i();
        int playerId = body["playerId"].i();
        std::string text = body["text"].s();

        if (text.empty()) return crow::response(400, "Empty message");

        ChatMessage msg;
        msg.game_session_id = gameId;
        msg.user_id = playerId;
        msg.message = text;

        std::time_t now = std::time(nullptr);
        msg.timestamp = std::to_string(now);

        DatabaseManager::addChatMessage(msg);

        return crow::response(200, "{\"status\":\"sent\"}");
    }

    crow::response ChatHeandler::GetMessages(int gameId)
    {
        auto messages = DatabaseManager::getChatMessages(gameId);

        crow::json::wvalue result;
        result = crow::json::wvalue::list();

        int i = 0;
        for (const auto& msg : messages)
        {
            crow::json::wvalue item;
            item["id"] = msg.id;
            item["playerId"] = msg.user_id;
            item["gameId"] = msg.game_session_id;
            item["text"] = msg.message;
            item["timestamp"] = msg.timestamp;
            result[i++] = std::move(item);
        }

        return crow::response(200, result);
    }
}