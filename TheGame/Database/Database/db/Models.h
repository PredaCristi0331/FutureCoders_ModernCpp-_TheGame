#pragma once
#include <string>

struct User {
    int id;                     
    std::string username;      
    std::string password;
};

struct GameSession {
    int id;
    std::string start_time;
    std::string end_time;
    std::string difficulty;
    int num_players;
    bool won;
    int cards_left_in_draw;
    int total_moves;
};

struct PlayerGameStats {
    int id;
    int user_id;
    int game_session_id;
    bool is_host;
    int final_cards_in_hand;
    int moves_played;
    bool won;
};

struct UserProfile {
    std::string username;

    int games_played;
    int games_won;
    int games_lost;

    double avg_cards_on_loss;

    int performance_score;
};
