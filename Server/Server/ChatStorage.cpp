import ChatMessage;
#include "ChatStorage.h"

namespace http
{
    bool ChatStorage::Initialize()
    {
        m_db.sync_schema();
        return true;
    }

    void ChatStorage::AddMessage(const ChatMessage& msg)
    {
        m_db.insert(msg);
    }

    std::vector<ChatMessage> ChatStorage::GetMessagesForGame(int gameId)
    {
        return m_db.get_all<ChatMessage>(
            sql::where(sql::c(&ChatMessage::GetGameId) == gameId),
            sql::order_by(&ChatMessage::GetId)
        );
    }
}