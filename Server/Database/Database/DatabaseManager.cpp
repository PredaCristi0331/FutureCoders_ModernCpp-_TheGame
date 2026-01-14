#include "DatabaseManager.h"
#include "sqlite_orm.h"
#include <regex>
#include <algorithm>
#include <numeric>

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
                make_column("performance_score", &User::performance_score, default_value(1))
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
            ),

            make_index("idx_pgs_user", &PlayerGameStats::user_id),
            make_index("idx_pgs_session", &PlayerGameStats::game_session_id),
            make_index("idx_chat_session", &ChatMessage::game_session_id)
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
    s.status = GameStatus::Waiting;
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
    if (sess.status != GameStatus::Waiting) return false;
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
    if (s.status != GameStatus::Waiting) return false;

    if (s.num_players < 2) return false;

    s.status = GameStatus::Running;
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
    if (s.status == GameStatus::Finished) return false;

    s.status = GameStatus::Finished;
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
        where(c(&GameSession::status) == GameStatus::Waiting &&
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

    auto stats = storage().get_all<PlayerGameStats>(
        where(c(&PlayerGameStats::user_id) == userId)
    );

    profile.games_played = static_cast<int>(stats.size());

    auto isWin = [](const PlayerGameStats& s) {
        return s.won;
        };

    auto isLoss = [](const PlayerGameStats& s) {
        return !s.won;
        };

    int gamesWon = std::count_if(stats.begin(), stats.end(), isWin);
    int gamesLost = std::count_if(stats.begin(), stats.end(), isLoss);

    long long sumCardsOnLoss = std::accumulate(
        stats.begin(), stats.end(), 0LL,
        [](long long acc, const PlayerGameStats& s) {
            return s.won ? acc : acc + s.final_cards_in_hand;
        }
    );

    int lossCount = gamesLost;

    profile.games_won = gamesWon;
    profile.games_lost = gamesLost;

    profile.avg_cards_on_loss = (lossCount > 0)
        ? static_cast<double>(sumCardsOnLoss) / lossCount
        : 0.0;

    auto calcWinRate = [](int won, int played) -> double {
        return (played > 0) ? static_cast<double>(won) / played : 0.0;
        };

    double winRate = calcWinRate(gamesWon, profile.games_played);

    auto scoreFromWinRate = [](double wr) {
        if (wr < 0.2) return 1;
        if (wr < 0.4) return 2;
        if (wr < 0.6) return 3;
        if (wr < 0.8) return 4;
        return 5;
        };

    int score = scoreFromWinRate(winRate);

    if (gamesLost > 0 && profile.avg_cards_on_loss >= 10.0) score -= 1;
    if (gamesLost > 0 && profile.avg_cards_on_loss >= 15.0) score -= 1;

    profile.performance_score = clampScore(score);
    return profile;
}



void DatabaseManager::recomputeAndUpdateUserStats(int userId) {
    auto profile = getUserProfile(userId);

    auto userSessions = storage().get_all<PlayerGameStats>(
        where(c(&PlayerGameStats::user_id) == userId)
    );

    std::int64_t totalSec = 0;
    for (const auto& p : userSessions) {
        auto sessions = storage().get_all<GameSession>(where(c(&GameSession::id) == p.game_session_id));
        if (!sessions.empty()) {
            const auto& s = sessions.front();
            if (s.status == GameStatus::Finished) {
                totalSec += std::max<std::int64_t>(0, s.duration_seconds);
            }
        }
    }

    auto uopt = getUserByIdSafe(userId);
    if (!uopt) return;

    auto u = *uopt;
    u.hours_played_seconds = totalSec;
    u.performance_score = profile.performance_score;

    storage().update(u);
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