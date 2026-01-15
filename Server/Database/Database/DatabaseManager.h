#pragma once

#include <optional>
#include <string>
#include <vector>
#include <cstdint>
#include "db/Models.h"

class DatabaseManager {
public:
    static void init(const std::string& dbPath = "game.db");

    static bool registerUser(const std::string& username, const std::string& password = "");
    static std::optional<User> loginUser(const std::string& username, const std::string& password = "");

    static int createWaitingSession(const std::string& created_at);
    static bool addPlayerToSession(int sessionId, int userId, bool isHost);
    static bool setSessionRunning(int sessionId, const std::string& start_time);
    static bool finishSession(int sessionId,
                              bool won,
                              int cards_left_in_draw,
                              int total_moves,
                              const std::string& end_time,
                              std::int64_t duration_seconds);

    static std::optional<int> findBestWaitingSessionForUser(int userId, int tolerance = 1);

    static void savePlayerStats(const PlayerGameStats& stats);
    static void recomputeAndUpdateUserStats(int userId);

    static std::optional<User> getUserByIdSafe(int userId);
    static UserProfile getUserProfile(int userId);

    static void addChatMessage(const ChatMessage& msg);
    static std::vector<ChatMessage> getChatMessages(int sessionId, int limit = 200);
};
