#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif

#include "DatabaseManager.h"
#include "db/sqlite_orm/sqlite_orm.h"
#include <regex>
#include <algorithm>
#include <numeric>
#include <iostream>

using namespace sqlite_orm;

namespace
{
    inline std::string& db_path_ref() {
        static std::string path = "game.db";
        return path;
    }

    inline auto& storage() {
        static auto stor = make_storage(
            db_path_ref(),

            make_table("users",
                make_column("id", &User::id, primary_key().autoincrement()),
                make_column("username", &User::username, unique()),
                make_column("password", &User::password),

                make_column("hours_played_seconds", &User::hours_played_seconds, default_value(0)),
                make_column("performance_score", &User::performance_score, default_value(1)),
                make_column("games_played", &User::games_played, default_value(0)),
                make_column("games_won", &User::games_won, default_value(0)),
                make_column("games_lost", &User::games_lost, default_value(0))
            ),

            make_table("game_sessions",
                make_column("id", &GameSession::id, primary_key().autoincrement()),

                make_column("created_at", &GameSession::created_at),
                make_column("start_time", &GameSession::start_time),
                make_column("end_time", &GameSession::end_time),

                make_column("status", &GameSession::status),
                make_column("num_players", &GameSession::num_players),

                make_column("won", &GameSession::won),
                make_column("cards_left_in_draw", &GameSession::cards_left_in_draw),
                make_column("total_moves", &GameSession::total_moves),

                make_column("duration_seconds", &GameSession::duration_seconds, default_value(0))
            ),

            make_table("player_game_stats",
                make_column("id", &PlayerGameStats::id, primary_key().autoincrement()),
                make_column("user_id", &PlayerGameStats::user_id),
                make_column("game_session_id", &PlayerGameStats::game_session_id),

                make_column("is_host", &PlayerGameStats::is_host),
                make_column("final_cards_in_hand", &PlayerGameStats::final_cards_in_hand),
                make_column("moves_played", &PlayerGameStats::moves_played),
                make_column("won", &PlayerGameStats::won),

                foreign_key(&PlayerGameStats::user_id).references(&User::id).on_delete.cascade(),
                foreign_key(&PlayerGameStats::game_session_id).references(&GameSession::id).on_delete.cascade()
            ),

            make_table("chat_messages",
                make_column("id", &ChatMessage::id, primary_key().autoincrement()),
                make_column("game_session_id", &ChatMessage::game_session_id),
                make_column("user_id", &ChatMessage::user_id),
                make_column("message", &ChatMessage::message),
                make_column("timestamp", &ChatMessage::timestamp),

                foreign_key(&ChatMessage::game_session_id).references(&GameSession::id).on_delete.cascade(),
                foreign_key(&ChatMessage::user_id).references(&User::id).on_delete.cascade()
            )

            // make_index("idx_pgs_user", &PlayerGameStats::user_id),
            // make_index("idx_pgs_session", &PlayerGameStats::game_session_id)
            // make_index("idx_chat_session", &ChatMessage::game_session_id)
        );


        stor.sync_schema(true);
        return stor;
    }

    static int clampScore(int s) {
        if (s < 1) return 1;
        if (s > 5) return 5;
        return s;
    }
}


void DatabaseManager::init(const std::string& dbPath)
{
    db_path_ref() = dbPath;
    storage();
}


bool DatabaseManager::registerUser(const std::string& username, const std::string& password) {
    static const std::regex usernameRegex("^[a-zA-Z0-9_]{3,16}$");
    if (!std::regex_match(username, usernameRegex))
        return false;

    auto existing = storage().get_all<User>(where(c(&User::username) == username));
    if (!existing.empty()) return false;

    User u{};
    u.username = username;
    u.password = password;
    u.hours_played_seconds = 0;
    u.performance_score = 1;
    u.games_played = 0;
    u.games_won = 0;
    u.games_lost = 0;

    try {
        storage().insert(u);
        return true;
    }
    catch (...) {
        return false;
    }
}


std::optional<User> DatabaseManager::loginUser(const std::string& username, const std::string& password) {
    auto users = storage().get_all<User>(where(c(&User::username) == username));
    if (users.empty()) return std::nullopt;

    User u = users.front();

    if (!u.password.empty() && u.password != password) return std::nullopt;

    return u;
}


int DatabaseManager::createWaitingSession(const std::string& created_at) {
    GameSession s{};
    s.created_at = created_at;
    s.status = static_cast<int>(GameStatus::Waiting);
    s.num_players = 0;
    s.won = false;
    s.cards_left_in_draw = 0;
    s.total_moves = 0;
    s.duration_seconds = 0;

    auto rowid = storage().insert(s);
    return static_cast<int>(rowid);
}

bool DatabaseManager::addPlayerToSession(int sessionId, int userId, bool isHost) {
    auto sessions = storage().get_all<GameSession>(where(c(&GameSession::id) == sessionId));
    if (sessions.empty()) return false;

    GameSession sess = sessions.front();
    if (sess.status != static_cast<int>(GameStatus::Waiting)) return false;
    if (sess.num_players >= 5) return false;

    auto existing = storage().get_all<PlayerGameStats>(
        where(c(&PlayerGameStats::game_session_id) == sessionId &&
            c(&PlayerGameStats::user_id) == userId)
    );
    if (!existing.empty()) return false;

    try {
        storage().begin_transaction();

        sess.num_players += 1;
        storage().update(sess);

        PlayerGameStats p{};
        p.user_id = userId;
        p.game_session_id = sessionId;
        p.is_host = isHost;
        p.final_cards_in_hand = 0;
        p.moves_played = 0;
        p.won = false;

        storage().insert(p);

        storage().commit();
        return true;
    }
    catch (...) {
        try { storage().rollback(); }
        catch (...) {}
        return false;
    }
}

bool DatabaseManager::setSessionRunning(int sessionId, const std::string& start_time) {
    auto sessions = storage().get_all<GameSession>(where(c(&GameSession::id) == sessionId));
    if (sessions.empty()) return false;

    auto s = sessions.front();
    if (s.status != static_cast<int>(GameStatus::Waiting)) return false;

    if (s.num_players < 2) return false;

    s.status = static_cast<int>(GameStatus::Running);
    s.start_time = start_time;

    try {
        storage().update(s);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool DatabaseManager::finishSession(int sessionId,
    bool won,
    int cards_left_in_draw,
    int total_moves,
    const std::string& end_time,
    std::int64_t duration_seconds) {
    auto sessions = storage().get_all<GameSession>(where(c(&GameSession::id) == sessionId));
    if (sessions.empty()) return false;

    auto s = sessions.front();
    if (s.status == static_cast<int>(GameStatus::Finished)) return false;

    s.status = static_cast<int>(GameStatus::Finished);
    s.won = won;
    s.cards_left_in_draw = cards_left_in_draw;
    s.total_moves = total_moves;
    s.end_time = end_time;
    s.duration_seconds = std::max<std::int64_t>(0, duration_seconds);

    try {
        storage().update(s);
        return true;
    }
    catch (...) {
        return false;
    }
}

std::optional<int> DatabaseManager::findBestWaitingSessionForUser(int userId, int tolerance) {
    auto uopt = getUserByIdSafe(userId);
    if (!uopt) return std::nullopt;
    int score = uopt->performance_score;

    auto waiting = storage().get_all<GameSession>(
        where(c(&GameSession::status) == static_cast<int>(GameStatus::Waiting) &&
            c(&GameSession::num_players) < 5)
    );
    if (waiting.empty()) return std::nullopt;

    struct Candidate { int sessionId; int diff; };
    std::vector<Candidate> cand;

    for (auto& sess : waiting) {
        auto hosts = storage().get_all<PlayerGameStats>(
            where(c(&PlayerGameStats::game_session_id) == sess.id &&
                c(&PlayerGameStats::is_host) == true)
        );

        int baseScore = score;
        if (!hosts.empty()) {
            auto hostUser = getUserByIdSafe(hosts.front().user_id);
            if (hostUser) baseScore = hostUser->performance_score;
        }

        int diff = std::abs(baseScore - score);
        cand.push_back({ sess.id, diff });
    }

    std::sort(cand.begin(), cand.end(), [](auto a, auto b) {
        if (a.diff != b.diff) return a.diff < b.diff;
        return a.sessionId < b.sessionId;
        });

    for (auto& cnd : cand) {
        if (cnd.diff <= tolerance) return cnd.sessionId;
    }
    return cand.front().sessionId;
}


void DatabaseManager::savePlayerStats(const PlayerGameStats& stats) {
    auto rows = storage().get_all<PlayerGameStats>(
        where(c(&PlayerGameStats::user_id) == stats.user_id &&
            c(&PlayerGameStats::game_session_id) == stats.game_session_id)
    );
    if (rows.empty()) {
        PlayerGameStats s = stats;
        s.id = 0;
        storage().insert(s);
        return;
    }

    auto s = rows.front();
    s.final_cards_in_hand = stats.final_cards_in_hand;
    s.moves_played = stats.moves_played;
    s.won = stats.won;

    storage().update(s);
}

std::optional<User> DatabaseManager::getUserByIdSafe(int userId) {
    try {
        return storage().get<User>(userId);
    }
    catch (...) {
        return std::nullopt;
    }
}


UserProfile DatabaseManager::getUserProfile(int userId) {
    UserProfile profile{};

    auto uopt = getUserByIdSafe(userId);
    if (!uopt) {
        profile.username = "(unknown)";
        return profile;
    }

    profile.username = uopt->username;
    profile.hours_played_seconds = uopt->hours_played_seconds;
    profile.games_played = uopt->games_played;
    profile.games_won = uopt->games_won;
    profile.games_lost = uopt->games_lost;
    profile.performance_score = uopt->performance_score;

    // avg_cards_on_loss is not stored in Users table and not used in UI currently.
    profile.avg_cards_on_loss = 0.0;

    return profile;
}



void DatabaseManager::updateUserStatsIncrement(int userId, bool won, int cardsLeft, std::int64_t durationSeconds) {
    auto uopt = getUserByIdSafe(userId);
    if (!uopt) {
        std::cerr << "[Stats] User not found: " << userId << std::endl;
        return;
    }

    auto u = *uopt;

    // INCREMENTAL UPDATE
    u.games_played++;
    if (won) {
        u.games_won++;
    }
    else {
        u.games_lost++;
    }
    u.hours_played_seconds += durationSeconds;


    // Score Calculation (Keep doing this via scan for accuracy on averages, or simplify?)
    // If we want to be purely incremental, we'd need to store total_lost_cards in User table. 
    // For now, let's just re-scan for the score to keep it robust-ish, but TRUST the counters we just incremented.
    // Actually, if we overwrite u.performance_score based on history scan, it might fluctuate if history is empty.
    // Let's keep the existing scan logic for score, but DO NOT overwrite games_played/won/lost from history.

    // Get stats for avg cards calculation
    auto userSessions = storage().get_all<PlayerGameStats>(
        where(c(&PlayerGameStats::user_id) == userId)
    );

    long long sumCardsOnLoss = 0;
    int actualLossesInHistory = 0;
    for (const auto& p : userSessions) {
        if (!p.won) {
            sumCardsOnLoss += p.final_cards_in_hand;
            actualLossesInHistory++;
        }
    }

    double avgCardsOnLoss = (actualLossesInHistory > 0)
        ? static_cast<double>(sumCardsOnLoss) / actualLossesInHistory
        : (cardsLeft > 0 ? (double)cardsLeft : 0.0); // Fallback to current game if history empty

    auto calcWinRate = [](int won, int played) -> double {
        return (played > 0) ? static_cast<double>(won) / played : 0.0;
        };

    double winRate = calcWinRate(u.games_won, u.games_played); // Use INCREMENTED values

    auto scoreFromWinRate = [](double wr) {
        if (wr < 0.2) return 1;
        if (wr < 0.4) return 2;
        if (wr < 0.6) return 3;
        if (wr < 0.8) return 4;
        return 5;
        };

    int score = scoreFromWinRate(winRate);

    if (u.games_lost > 0 && avgCardsOnLoss >= 10.0) score -= 1;
    if (u.games_lost > 0 && avgCardsOnLoss >= 15.0) score -= 1;

    u.performance_score = clampScore(score);

    storage().update(u);

    std::cout << "[Stats] Incremented user " << userId << ": games_played=" << u.games_played
        << ", games_won=" << u.games_won << ", games_lost=" << u.games_lost
        << ", performance_score=" << u.performance_score 
        << ", total_minutes_played=" << (u.hours_played_seconds / 60) << std::endl;
}

void DatabaseManager::addChatMessage(const ChatMessage& msg) {
    ChatMessage m = msg;
    m.id = 0;
    storage().insert(m);
}

std::vector<ChatMessage> DatabaseManager::getChatMessages(int sessionId, int limit) {
    if (limit <= 0) limit = 200;

    auto msgs = storage().get_all<ChatMessage>(
        where(c(&ChatMessage::game_session_id) == sessionId),
        order_by(&ChatMessage::id).desc(),
        sqlite_orm::limit(limit)
    );
    return msgs;
}

std::optional<User> DatabaseManager::getUserByUsername(const std::string& username) {
    auto users = storage().get_all<User>(where(c(&User::username) == username));
    if (users.empty()) return std::nullopt;
    return users.front();
}