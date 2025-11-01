#pragma once
#include <string>
#include <vector>

struct User {
    int id;
    std::string username;
    std::string password;
};

struct Game {
    int id;
    std::string player;
    int score;
    std::string date;
};

class DatabaseManager {
private:
    std::string databasePath;

public:
    DatabaseManager(const std::string& dbPath);

    void addUser(const std::string& username, const std::string& password);
    bool userExists(const std::string& username);

    void addGameResult(const std::string& player, int score);
    std::vector<Game> getGamesByUser(const std::string& username);
};

