#include "DatabaseManager.h"
#include "db/sqlite_orm/sqlite_orm.h"
#include "Statistics.h"

using namespace sqlite_orm;

static auto makeStorage(const std::string& path) {
    return make_storage(path,

        make_table("users",
            make_column("id", &User::id, autoincrement(), primary_key()),
            make_column("username", &User::username, unique()),
            make_column("password_hash", &User::passwordHash),
            make_column("created_at", &User::createdAt)
        ),

        make_table("game_sessions",
            make_column("id", &GameSession::id, autoincrement(), primary_key()),
            make_column("start_time", &GameSession::startTime),
            make_column("end_time", &GameSession::endTime),
            make_column("num_players", &GameSession::numPlayers),
            make_column("custom_level", &GameSession::customLevel),
            make_column("team_won", &GameSession::teamWon),
            make_column("cards_left_deck", &GameSession::cardsLeftDeck)
        ),

        make_table("player_game_stats",
            make_column("id", &PlayerGameStats::id, autoincrement(), primary_key()),
            make_column("user_id", &PlayerGameStats::userId),
            make_column("game_id", &PlayerGameStats::gameId),
            make_column("cards_in_hand_end", &PlayerGameStats::cardsInHandEnd),
            make_column("performance_score", &PlayerGameStats::performanceScore),
            make_column("was_host", &PlayerGameStats::wasHost)
        )
    );
}


DatabaseManager::DatabaseManager(const std::string& dbPath)
    : storage(makeStorage(dbPath)) {
}


void DatabaseManager::init() {
    storage.sync_schema();
}


int DatabaseManager::createUser(const std::string& username,
    const std::string& passwordHash) {
    return -1;
}

std::optional<User> DatabaseManager::getUserByName(const std::string& username) {
    return std::nullopt;
}

int DatabaseManager::createGameSession(int numPlayers, int customLevel) {
    return -1;
}

void DatabaseManager::finishGame(int gameId, bool teamWon, int cardsLeftDeck) {
}

void DatabaseManager::addPlayerGameStats(int gameId,
    int userId,
    int cardsInHandEnd,
    int performanceScore,
    bool wasHost) {
}

std::optional<UserProfileStats> DatabaseManager::getUserProfile(int userId) {
    return std::nullopt;
}
