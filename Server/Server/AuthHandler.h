#pragma once
#include <crow.h>
#include <string>
#include <map>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <functional>

namespace http
{
    struct User
    {
        std::string username;
        std::string sessionToken;
        std::string hashedPassword;
        int gamesPlayed = 0;
    };

    class AuthHandler
    {
    private:
        std::map<std::string, User> m_users; // username -> User
        std::map<std::string, std::string> m_tokens; // token -> username
        std::mutex m_mutex;

        std::string GenerateToken(const std::string& username);

    public:
        crow::response Register(const crow::request& req);
        crow::response Login(const crow::request& req);
        crow::response Logout(const crow::request& req);
        bool ValidateToken(const std::string& token);

        std::string HashPassword(const std::string& password);
        crow::response RegisterWithPassword(const crow::request& req);
        bool CheckPassword(const std::string& username, const std::string& password);
    };
}
