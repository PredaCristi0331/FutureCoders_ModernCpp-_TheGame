#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : databasePath(dbPath)
{
    // TODO: initialize database connection
    std::cout << "DatabaseManager created with path: " << dbPath << std::endl;
}

void DatabaseManager::addUser(const std::string& username, const std::string& password)
{
    // TODO: insert user into database
}

bool DatabaseManager::userExists(const std::string& username)
{
    // TODO: check if user exists
    return false;
}

void DatabaseManager::addGameResult(const std::string& player, int score)
{
    // TODO: insert game result into database
}

std::vector<Game> DatabaseManager::getGamesByUser(const std::string& username)
{
    // TODO: return all games played by a given user
    return {};
}
