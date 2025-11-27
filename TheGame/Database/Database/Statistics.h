#pragma once

#include <string>

struct User {
    int id;
    std::string username;
    std::string passwordHash;
    std::string createdAt;
};

struct GameSession {
    int id;
    std::string startTime;
    std::string endTime;
    int numPlayers;
    int customLevel;
    int teamWon;
    int cardsLeftDeck;
};

struct PlayerGameStats {
    int id;
    int userId;
    int gameId;
    int cardsInHandEnd;
    int performanceScore;
    int wasHost;
};

struct UserProfileStats {
    int userId;
    std::string username;
    int totalGames;
    int gamesWon;
    double winRate;
    double avgCardsLostGames;
    double avgPerformance;
};