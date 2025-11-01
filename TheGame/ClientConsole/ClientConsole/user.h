#ifndef USER_H
#define USER_H

#include <string>

struct User {
    int id;
    std::string username;
    int gamesPlayed;
    int gamesWon;
    double hoursPlayed;
    int performanceScore;

    User(int id = -1,
        std::string username = "",
        int gamesPlayed = 0,
        int gamesWon = 0,
        double hoursPlayed = 0.0,
        int performanceScore = 1)
        : id(id),
        username(std::move(username)),
        gamesPlayed(gamesPlayed),
        gamesWon(gamesWon),
        hoursPlayed(hoursPlayed),
        performanceScore(performanceScore) {
    }
};

#endif // USER_H
