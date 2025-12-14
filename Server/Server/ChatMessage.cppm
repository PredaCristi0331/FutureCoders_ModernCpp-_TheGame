export module ChatMessage;

#include <string>

namespace chat {

    export class ChatMessage
    {
        int m_id{};
        int m_playerId{};
        int m_gameId{};
        std::string m_text;
        std::string m_timestamp;

    public:
        int GetId() const;
        void SetId(int id);

        int GetPlayerId() const;
        void SetPlayerId(int id);

        int GetGameId() const;
        void SetGameId(int id);

        const std::string& GetText() const;
        void SetText(const std::string& text);

        const std::string& GetTimestamp() const;
        void SetTimestamp(const std::string& ts);
    };
}