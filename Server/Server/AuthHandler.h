#pragma once
#include <crow.h>
#include <string>
#include <map>
#include <mutex>

namespace http
{
    struct User
    {
        std::string username;
        std::string sessionToken;
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
    };
}
