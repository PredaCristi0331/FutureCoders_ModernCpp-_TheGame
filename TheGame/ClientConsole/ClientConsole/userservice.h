#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "database.h"
#include <string>

class UserService {
public:
    explicit UserService(Database& db);

    std::string registerUser(const std::string& username);
    std::string loginUser(const std::string& username);

private:
    Database& database;
};

#endif // USER_SERVICE_H
