#pragma once

#include <string>
#include <optional>
#include <memory>
#include "Statistics.h"

// Forward declaration - actual storage type defined in .cpp
struct StorageImpl;

class DatabaseManager {
public:
	explicit DatabaseManager(const std::string& dbPath = "thegame.db");
	~DatabaseManager();

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
    std::unique_ptr<StorageImpl> storage;
};


