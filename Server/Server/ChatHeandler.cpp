#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "ChatHeandler.h"
#include <ctime>

using chat::ChatMessage;

namespace http
{
    ChatHeandler::ChatHeandler(ChatStorage& storage)
        : m_storage(storage)
    {
    }
    crow::response ChatHeandler::PostMessage(const crow::request& req)
    {
        auto body = crow::json::load(req.body);

        int gameId = body["gameId"].i();
        int playerId = body["playerId"].i();
        std::string text = body["text"].s();

        ChatMessage msg;
        msg.SetGameId(gameId);
        msg.SetPlayerId(playerId);
        msg.SetText(text);

        std::time_t now = std::time(nullptr);
        msg.SetTimestamp(std::to_string(now));

        m_storage.AddMessage(msg);

        return crow::response(200, "{\"status\":\"sent\"}");
    }

    crow::response ChatHeandler::GetMessages(int gameId)
    {
        auto messages = m_storage.GetMessagesForGame(gameId);

        crow::json::wvalue result;
        result = crow::json::wvalue::list();

        int i = 0;
        for (const auto& msg : messages)
        {
            crow::json::wvalue item;
            item["id"] = msg.GetId();
            item["playerId"] = msg.GetPlayerId();
            item["gameId"] = msg.GetGameId();
            item["text"] = msg.GetText();
            item["timestamp"] = msg.GetTimestamp();
            result[i++] = std::move(item);
        }

        return crow::response(200, result);
    }
}