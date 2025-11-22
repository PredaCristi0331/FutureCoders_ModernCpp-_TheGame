#pragma once

#include <string>
#include <optional>
#include "db/sqlite_orm/sqlite_orm.h"
#include "Statistics.h"

class DatabaseManager {
public:
	explicit DatabaseManager(const std::string& dbPath = "thegame.db");

	void init();

	int createUser(const std::string& username, const std::string& passwordHash);
	std::optional<User> getUserByName(const std::string& username);

    int createGameSession(int numPlayers, int customLevel);
    void finishGame(int gameId, bool teamWon, int cardsLeftDeck);

    void addPlayerGameStats(int gameId,
        int userId,
        int cardsInHandEnd,
        int performanceScore,
        bool wasHost);

    std::optional<UserProfileStats> getUserProfile(int userId);
private:
    using Storage = decltype(sqlite_orm::make_storage(
        "",
        sqlite_orm::make_table("users"),
        sqlite_orm::make_table("game_sessions"),
        sqlite_orm::make_table("player_game_stats")
    ));
	Storage storage;
};

