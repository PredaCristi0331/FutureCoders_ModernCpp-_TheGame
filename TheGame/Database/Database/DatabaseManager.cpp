#include "DatabaseManager.h"
#include "db/sqlite_orm/sqlite_orm.h"
#include <ctime>
#include <iostream>

using namespace sqlite_orm;

// Define the actual storage type in .cpp
struct StorageImpl {
    decltype(make_storage("",
        make_table("users",
            make_column("id", &User::id, primary_key()),
            make_column("username", &User::username, unique()),
            make_column("password_hash", &User::passwordHash),
            make_column("created_at", &User::createdAt)
        ),
        make_table("game_sessions",
            make_column("id", &GameSession::id, primary_key()),
            make_column("start_time", &GameSession::startTime),
            make_column("end_time", &GameSession::endTime),
            make_column("num_players", &GameSession::numPlayers),
            make_column("custom_level", &GameSession::customLevel),
            make_column("team_won", &GameSession::teamWon),
            make_column("cards_left_deck", &GameSession::cardsLeftDeck)
        ),
        make_table("player_game_stats",
            make_column("id", &PlayerGameStats::id, primary_key()),
            make_column("user_id", &PlayerGameStats::userId),
            make_column("game_id", &PlayerGameStats::gameId),
            make_column("cards_in_hand_end", &PlayerGameStats::cardsInHandEnd),
            make_column("performance_score", &PlayerGameStats::performanceScore),
            make_column("was_host", &PlayerGameStats::wasHost)
        )
    )) storage;

    StorageImpl(const std::string& path) 
        : storage(make_storage(path,
            make_table("users",
                make_column("id", &User::id, primary_key()),
                make_column("username", &User::username, unique()),
                make_column("password_hash", &User::passwordHash),
                make_column("created_at", &User::createdAt)
            ),
            make_table("game_sessions",
                make_column("id", &GameSession::id, primary_key()),
                make_column("start_time", &GameSession::startTime),
                make_column("end_time", &GameSession::endTime),
                make_column("num_players", &GameSession::numPlayers),
                make_column("custom_level", &GameSession::customLevel),
                make_column("team_won", &GameSession::teamWon),
                make_column("cards_left_deck", &GameSession::cardsLeftDeck)
            ),
            make_table("player_game_stats",
                make_column("id", &PlayerGameStats::id, primary_key()),
                make_column("user_id", &PlayerGameStats::userId),
                make_column("game_id", &PlayerGameStats::gameId),
                make_column("cards_in_hand_end", &PlayerGameStats::cardsInHandEnd),
                make_column("performance_score", &PlayerGameStats::performanceScore),
                make_column("was_host", &PlayerGameStats::wasHost)
            )
        )) {}
};

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : storage(std::make_unique<StorageImpl>(dbPath)) {
}

DatabaseManager::~DatabaseManager() = default;

void DatabaseManager::init() {
    storage->storage.sync_schema();
}

int DatabaseManager::createUser(const std::string& username, const std::string& passwordHash) {
    try {
        auto now = std::to_string(std::time(nullptr));
        User user{ -1, username, passwordHash, now };
        return storage->storage.insert(user);
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating user: " << e.what() << std::endl;
        return -1;
    }
}

std::optional<User> DatabaseManager::getUserByName(const std::string& username) {
    try {
        auto users = storage->storage.get_all<User>(where(c(&User::username) == username));
        if (users.empty()) {
            return std::nullopt;
        }
        return users.front();
    }
    catch (const std::exception& e) {
        std::cerr << "Error getting user: " << e.what() << std::endl;
        return std::nullopt;
    }
}

int DatabaseManager::createGameSession(int numPlayers, int customLevel) {
    try {
        auto now = std::to_string(std::time(nullptr));
        GameSession session{ -1, now, "", numPlayers, customLevel, 0, 0 };
        return storage->storage.insert(session);
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating game session: " << e.what() << std::endl;
        return -1;
    }
}

void DatabaseManager::finishGame(int gameId, bool teamWon, int cardsLeftDeck) {
    try {
        auto session = storage->storage.get<GameSession>(gameId);
        session.endTime = std::to_string(std::time(nullptr));
        session.teamWon = teamWon ? 1 : 0;
        session.cardsLeftDeck = cardsLeftDeck;
        storage->storage.update(session);
    }
    catch (const std::exception& e) {
        std::cerr << "Error finishing game: " << e.what() << std::endl;
    }
}

void DatabaseManager::addPlayerGameStats(int gameId, int userId, int cardsInHandEnd, int performanceScore, bool wasHost) {
    try {
        PlayerGameStats stats{ -1, userId, gameId, cardsInHandEnd, performanceScore, wasHost ? 1 : 0 };
        storage->storage.insert(stats);
    }
    catch (const std::exception& e) {
        std::cerr << "Error adding player stats: " << e.what() << std::endl;
    }
}

std::optional<UserProfileStats> DatabaseManager::getUserProfile(int userId) {
    try {
        auto user = storage->storage.get<User>(userId);
        auto playerStats = storage->storage.get_all<PlayerGameStats>(where(c(&PlayerGameStats::userId) == userId));

        if (playerStats.empty()) {
            return UserProfileStats{ userId, user.username, 0, 0, 0.0, 0.0, 0.0 };
        }

        int totalGames = static_cast<int>(playerStats.size());
        int gamesWon = 0;
        int totalCardsLost = 0;
        int totalPerformance = 0;
        int lostGamesCount = 0;

        for (const auto& stats : playerStats) {
            auto session = storage->storage.get<GameSession>(stats.gameId);
            if (session.teamWon == 1) {
                gamesWon++;
            }
            else {
                totalCardsLost += stats.cardsInHandEnd;
                lostGamesCount++;
            }
            totalPerformance += stats.performanceScore;
        }

        double winRate = totalGames > 0 ? (static_cast<double>(gamesWon) / totalGames) : 0.0;
        double avgCardsLostGames = lostGamesCount > 0 ? (static_cast<double>(totalCardsLost) / lostGamesCount) : 0.0;
        double avgPerformance = totalGames > 0 ? (static_cast<double>(totalPerformance) / totalGames) : 0.0;

        return UserProfileStats{ userId, user.username, totalGames, gamesWon, winRate, avgCardsLostGames, avgPerformance };
    }
    catch (const std::exception& e) {
        std::cerr << "Error getting user profile: " << e.what() << std::endl;
        return std::nullopt;
    }
}
