#pragma once
#include <string>
#include <cstdint>

struct User {
    int id = 0;                     
    std::string username;      
    std::string password;

    std::int64_t hours_played_seconds = 0;
    int performance_score = 1;
};

enum class GameStatus : int {
    Waiting = 0,
    Running = 1,
    Finished = 2
};

struct GameSession {
    int id = 0;

    std::string created_at;
    std::string start_time;
    std::string end_time;

    int status = 0; // 0=Waiting, 1=Running, 2=Finished

    int num_players = 0;
    bool won = false;

    int cards_left_in_draw = 0;
    int total_moves = 0;

    std::int64_t duration_seconds = 0;
};

struct PlayerGameStats {
    int id;
    int user_id;
    int game_session_id;

    bool is_host = false;

    int final_cards_in_hand = 0;
    int moves_played = 0;

    bool won = false;
};

struct ChatMessage {
    int id = 0;
    int game_session_id = 0;
    int user_id = 0;

    std::string message;
    std::string timestamp;
};

struct UserProfile {
    std::string username;

    int games_played = 0;
    int games_won = 0;
    int games_lost = 0;

    double avg_cards_on_loss = 0.0;

    int performance_score = 1;
    std::int64_t hours_played_seconds = 0;
};
