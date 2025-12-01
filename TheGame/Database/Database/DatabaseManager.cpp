#include "DatabaseManager.h"
#include "sqlite_orm.h"
#include <vector>

namespace
{
    inline auto& storage()
    {
        static auto storage = sqlite_orm::make_storage(
            "game.db",

            sqlite_orm::make_table("users",
                sqlite_orm::make_column("id", &User::id,
                    sqlite_orm::primary_key().autoincrement()),
                sqlite_orm::make_column("username", &User::username,
                    sqlite_orm::unique()),
                sqlite_orm::make_column("password", &User::password)
            ),

            sqlite_orm::make_table("game_sessions",
                sqlite_orm::make_column("id", &GameSession::id,
                    sqlite_orm::primary_key().autoincrement()),
                sqlite_orm::make_column("start_time", &GameSession::start_time),
                sqlite_orm::make_column("end_time", &GameSession::end_time),
                sqlite_orm::make_column("difficulty", &GameSession::difficulty),
                sqlite_orm::make_column("num_players", &GameSession::num_players),
                sqlite_orm::make_column("won", &GameSession::won),
                sqlite_orm::make_column("cards_left_in_draw", &GameSession::cards_left_in_draw),
                sqlite_orm::make_column("total_moves", &GameSession::total_moves)
            ),

            sqlite_orm::make_table("player_game_stats",
                sqlite_orm::make_column("id", &PlayerGameStats::id,
                    sqlite_orm::primary_key().autoincrement()),
                sqlite_orm::make_column("user_id", &PlayerGameStats::user_id),
                sqlite_orm::make_column("game_session_id", &PlayerGameStats::game_session_id),
                sqlite_orm::make_column("is_host", &PlayerGameStats::is_host),
                sqlite_orm::make_column("final_cards_in_hand", &PlayerGameStats::final_cards_in_hand),
                sqlite_orm::make_column("moves_played", &PlayerGameStats::moves_played),
                sqlite_orm::make_column("won", &PlayerGameStats::won)
            )
        );

        storage.sync_schema();
        return storage;
    }
}


void DatabaseManager::init(const std::string& dbPath)
{
    (void)dbPath;
    storage();
}


bool DatabaseManager::registerUser(const std::string& username,
    const std::string& password)
{
    auto existing = storage().get_all<User>(
        sqlite_orm::where(sqlite_orm::c(&User::username) == username)
    );

    if (!existing.empty()) {
        return false;
    }

    User u{};
    u.id = 0;
    u.username = username;
    u.password = password;

    storage().insert(u);
    return true;
}


std::optional<User> DatabaseManager::loginUser(const std::string& username,
    const std::string& password)
{
    auto users = storage().get_all<User>(
        sqlite_orm::where(sqlite_orm::c(&User::username) == username)
    );

    if (users.empty()) {
        return std::nullopt;
    }

    User u = users.front();
    if (u.password != password) {
        return std::nullopt;
    }
    return u;
}


int DatabaseManager::createGameSession(const GameSession& session)
{
    GameSession s = session;
    s.id = 0;

    auto rowid = storage().insert(s);
    return static_cast<int>(rowid);
}


void DatabaseManager::savePlayerStats(const PlayerGameStats& stats)
{
    PlayerGameStats s = stats;
    s.id = 0;

    storage().insert(s);
}


User DatabaseManager::getUserById(int userId)
{
    return storage().get<User>(userId);
}


UserProfile DatabaseManager::getUserProfile(int userId)
{
    UserProfile profile{};

    try {
        User user = storage().get<User>(userId);
        profile.username = user.username;
    }
    catch (...) {
        profile.username = "(unknown)";
    }

    auto stats = storage().get_all<PlayerGameStats>(
        sqlite_orm::where(sqlite_orm::c(&PlayerGameStats::user_id) == userId)
    );

    profile.games_played = static_cast<int>(stats.size());

    int gamesWon = 0;
    int gamesLost = 0;
    int sumCardsOnLoss = 0;
    int lossCount = 0;

    for (const auto& s : stats) {
        if (s.won) {
            gamesWon++;
        }
        else {
            gamesLost++;
            sumCardsOnLoss += s.final_cards_in_hand;
            lossCount++;
        }
    }

    profile.games_won = gamesWon;
    profile.games_lost = gamesLost;

    if (lossCount > 0) {
        profile.avg_cards_on_loss =
            static_cast<double>(sumCardsOnLoss) / lossCount;
    }
    else {
        profile.avg_cards_on_loss = 0.0;
    }

    double winRate = 0.0;
    if (profile.games_played > 0) {
        winRate = static_cast<double>(gamesWon) / profile.games_played;
    }

    int score = 0;
    if (profile.games_played > 0) {
        if (winRate < 0.2)       score = 1;
        else if (winRate < 0.4)  score = 2;
        else if (winRate < 0.6)  score = 3;
        else if (winRate < 0.8)  score = 4;
        else                     score = 5;
    }
    profile.performance_score = score;

    return profile;
}
