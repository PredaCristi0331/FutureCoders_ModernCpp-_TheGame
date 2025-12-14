#include <string>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

import Player;
import ChatMessage;

namespace sql = sqlite_orm;

namespace http
{
    inline auto CreateStorage(const std::string& filename)
    {
        return sql::make_storage(
            filename,

            // ---- Players ----
            sql::make_column(
                "id",
                &game::Player::SetId,
                &game::Player::GetId,
                sql::primary_key().autoincrement()
            ),
            sql::make_column(
                "name",
                &game::Player::SetName,
                &game::Player::GetName
            ),

            // ---- Chat messages ----
            sql::make_table(
                "ChatMessages",
                sql::make_column("id", &ChatMessage::SetId, &ChatMessage::GetId,
                    sql::primary_key().autoincrement()),
                sql::make_column("playerId", &ChatMessage::SetPlayerId, &ChatMessage::GetPlayerId),
                sql::make_column("gameId", &ChatMessage::SetGameId, &ChatMessage::GetGameId),
                sql::make_column("message", &ChatMessage::SetText, &ChatMessage::GetText),
                sql::make_column("timestamp", &ChatMessage::SetTimestamp, &ChatMessage::GetTimestamp)
            )
        );
    }

    using Storage = decltype(CreateStorage(""));

    class ChatStorage
    {
    public:
        bool Initialize();

        void AddMessage(const ChatMessage& msg);
        std::vector<ChatMessage> GetMessagesForGame(int gameId);

    private:
        const std::string kDbFile{ "chat.sqlite" };
        Storage m_db = CreateStorage(kDbFile);
    };
}