#pragma once

#include <optional>
#include <string>
#include "Models.h"

class DatabaseManager {
public:
    static void init(const std::string& dbPath = "game.db");

    static bool registerUser(const std::string& username,
        const std::string& password);

    static std::optional<User> loginUser(const std::string& username,
        const std::string& password);

    static int createGameSession(const GameSession& session);

    static void savePlayerStats(const PlayerGameStats& stats);

    static User getUserById(int userId);

    static UserProfile getUserProfile(int userId);
};
