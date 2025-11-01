#ifndef DATABASE_H
#define DATABASE_H

#include "user.h"
#include <sqlite_orm/sqlite_orm.h>
#include <memory>

class Database {
public:
    Database(const std::string& dbPath);

    void sync();
    bool addUser(const User& user);
    std::shared_ptr<User> getUser(const std::string& username);
    bool userExists(const std::string& username);

private:
    decltype(sqlite_orm::make_storage("")) storage;
};

#endif // DATABASE_H
