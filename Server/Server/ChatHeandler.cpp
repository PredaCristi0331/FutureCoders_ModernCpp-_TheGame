#include "ChatHeandler.h"
import ChatMessage;
#include "json.hpp"
#include <ctime>

using json = nlohmann::json;

namespace http
{
    ChatHeandler::ChatHeandler(ChatStorage& storage)
        : m_storage(storage)
    {
    }

    crow::response ChatHeandler::PostMessage(const crow::request& req)
    {
        auto body = json::parse(req.body);

        ChatMessage msg;
        msg.SetPlayerId(body["playerId"]);
        msg.SetGameId(body["gameId"]);
        msg.SetText(body["text"]);

        // simple timestamp
        msg.SetTimestamp(std::to_string(std::time(nullptr)));

        m_storage.AddMessage(msg);

        return crow::response(200, "Message stored");
    }

    crow::response ChatHeandler::GetMessages(int gameId)
    {
        json result = json::array();

        for (const auto& msg : m_storage.GetMessagesForGame(gameId))
        {
            result.push_back({
                {"playerId", msg.GetPlayerId()},
                {"text", msg.GetText()},
                {"time", msg.GetTimestamp()}
                });
        }

        return crow::response(result.dump());
    }
}