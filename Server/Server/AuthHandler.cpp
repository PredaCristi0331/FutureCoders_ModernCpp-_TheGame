#include "AuthHandler.h"
#include <random>
#include <sstream>
#include <iomanip>

namespace http
{
    std::string AuthHandler::GenerateToken(const std::string& username)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);

        std::stringstream ss;
        ss << username << "_";
        for (int i = 0; i < 16; i++)
        {
            ss << std::hex << dis(gen);
        }
        return ss.str();
    }

    crow::response AuthHandler::Register(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto body = crow::json::load(req.body);
        std::string username = "";
        if (body.has("username")) {
            username = body["username"].s();
        }

        if (username.empty())
        {
            return crow::response(400, "Username required");
        }

        if (m_users.find(username) != m_users.end())
        {
            return crow::response(409, "User already exists");
        }

        User user;
        user.username = username;
        user.sessionToken = GenerateToken(username);

        m_users[username] = user;
        m_tokens[user.sessionToken] = username;

        crow::json::wvalue response;
        response["username"] = username;
        response["token"] = user.sessionToken;
        response["status"] = "registered";

        return crow::response(200, response);
    }

    crow::response AuthHandler::Login(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto body = crow::json::load(req.body);
        std::string username = "";
        if (body.has("username")) {
            username = body["username"].s();
        }

        if (username.empty())
        {
            return crow::response(400, "Username required");
        }

        auto it = m_users.find(username);
        if (it == m_users.end())
        {
            return crow::response(404, "User not found");
        }

        // Generate new token on login
        std::string newToken = GenerateToken(username);
        
        // Remove old token
        m_tokens.erase(it->second.sessionToken);
        
        // Update user with new token
        it->second.sessionToken = newToken;
        m_tokens[newToken] = username;

        crow::json::wvalue response;
        response["username"] = username;
        response["token"] = newToken;
        response["status"] = "logged_in";

        return crow::response(200, response);
    }

    crow::response AuthHandler::Logout(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto body = crow::json::load(req.body);
        std::string token = "";
        if (body.has("token")) {
            token = body["token"].s();
        }

        if (token.empty())
        {
            return crow::response(400, "Token required");
        }

        auto tokenIt = m_tokens.find(token);
        if (tokenIt != m_tokens.end())
        {
            std::string username = tokenIt->second;
            m_tokens.erase(tokenIt);
            
            auto userIt = m_users.find(username);
            if (userIt != m_users.end())
            {
                userIt->second.sessionToken = "";
            }

            return crow::response(200, "{\"status\":\"logged_out\"}");
        }

        return crow::response(404, "Invalid token");
    }

    bool AuthHandler::ValidateToken(const std::string& token)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_tokens.find(token) != m_tokens.end();
    }
}
