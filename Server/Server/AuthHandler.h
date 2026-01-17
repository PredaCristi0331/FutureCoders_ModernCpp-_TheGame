#pragma once
#include <crow.h>
#include <string>
#include <map>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <functional>
#include "../Database/Database/DatabaseManager.h" // Includes db/Models.h

namespace http
{
    class AuthHandler
    {
    private:
        std::map<std::string, std::string> m_tokens; // token -> username
        std::mutex m_mutex;

        std::string GenerateToken(const std::string& username);

    public:
        crow::response Register(const crow::request& req);
        crow::response Login(const crow::request& req);
        crow::response Logout(const crow::request& req);
        bool ValidateToken(const std::string& token);

        // Helper not strictly needed if DB handles it, but maybe useful?
        // std::string HashPassword(const std::string& password); 
        
        crow::response GetProfile(const std::string& token);
    };
}
