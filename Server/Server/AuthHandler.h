#pragma once
#include <crow.h>
#include <string>
#include <map>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <functional>
#include "../Database/Database/DatabaseManager.h"

namespace http
{
    class AuthHandler
    {
    private:
        std::map<std::string, std::string> m_tokens;
        std::mutex m_mutex;

        std::string GenerateToken(const std::string& username);

    public:
        crow::response Register(const crow::request& req);
        crow::response Login(const crow::request& req);
        crow::response Logout(const crow::request& req);
        bool ValidateToken(const std::string& token);

        crow::response GetProfile(const std::string& token);
    };
}
