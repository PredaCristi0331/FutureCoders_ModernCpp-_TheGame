#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "ChatStorage.h"

namespace http
{
    bool ChatStorage::Initialize()
    {
        m_db.sync_schema();
        return true;
    }

    void ChatStorage::AddMessage(const chat::ChatMessage& msg)
    {
        m_db.insert(msg);
    }

    std::vector<chat::ChatMessage> ChatStorage::GetMessagesForGame(int gameId)
    {
        using namespace sqlite_orm;
        return m_db.get_all<chat::ChatMessage>(
            where(c(&chat::ChatMessage::GetGameId) == gameId)
        );
    }
}
