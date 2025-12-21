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

        // Validate JSON body
        auto body = crow::json::load(req.body);
        if (!body)
        {
            crow::json::wvalue error;
            error["error"] = "Invalid JSON format";
            error["message"] = "Request body must be valid JSON";
            return crow::response(400, error);
        }

        // Validate username field exists
        if (!body.has("username"))
        {
            crow::json::wvalue error;
            error["error"] = "Missing field";
            error["message"] = "Username field is required";
            return crow::response(400, error);
        }

        std::string username = body["username"].s();

        // Validate username not empty
        if (username.empty())
        {
            crow::json::wvalue error;
            error["error"] = "Invalid username";
            error["message"] = "Username cannot be empty";
            return crow::response(400, error);
        }

        // Validate username length
        if (username.length() < 3)
        {
            crow::json::wvalue error;
            error["error"] = "Invalid username";
            error["message"] = "Username must be at least 3 characters long";
            return crow::response(400, error);
        }

        // Check if user already exists
        if (m_users.find(username) != m_users.end())
        {
            crow::json::wvalue error;
            error["error"] = "User exists";
            error["message"] = "A user with this username already exists";
            return crow::response(409, error);
        }

        // Create new user
        User user;
        user.username = username;
        user.sessionToken = GenerateToken(username);

        m_users[username] = user;
        m_tokens[user.sessionToken] = username;

        crow::json::wvalue response;
        response["username"] = username;
        response["token"] = user.sessionToken;
        response["status"] = "registered";
        response["message"] = "User registered successfully";

        return crow::response(200, response);
    }

    crow::response AuthHandler::Login(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        // Validate JSON body
        auto body = crow::json::load(req.body);
        if (!body)
        {
            crow::json::wvalue error;
            error["error"] = "Invalid JSON format";
            error["message"] = "Request body must be valid JSON";
            return crow::response(400, error);
        }

        // Validate username field exists
        if (!body.has("username"))
        {
            crow::json::wvalue error;
            error["error"] = "Missing field";
            error["message"] = "Username field is required";
            return crow::response(400, error);
        }

        std::string username = body["username"].s();

        // Validate username not empty
        if (username.empty())
        {
            crow::json::wvalue error;
            error["error"] = "Invalid username";
            error["message"] = "Username cannot be empty";
            return crow::response(400, error);
        }

        // Check if user exists
        auto it = m_users.find(username);
        if (it == m_users.end())
        {
            crow::json::wvalue error;
            error["error"] = "Authentication failed";
            error["message"] = "User not found. Please register first";
            return crow::response(404, error);
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
        response["message"] = "Login successful";

        return crow::response(200, response);
    }

    crow::response AuthHandler::Logout(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        // Validate JSON body
        auto body = crow::json::load(req.body);
        if (!body)
        {
            crow::json::wvalue error;
            error["error"] = "Invalid JSON format";
            error["message"] = "Request body must be valid JSON";
            return crow::response(400, error);
        }

        // Validate token field exists
        if (!body.has("token"))
        {
            crow::json::wvalue error;
            error["error"] = "Missing field";
            error["message"] = "Token field is required";
            return crow::response(400, error);
        }

        std::string token = body["token"].s();

        // Validate token not empty
        if (token.empty())
        {
            crow::json::wvalue error;
            error["error"] = "Invalid token";
            error["message"] = "Token cannot be empty";
            return crow::response(400, error);
        }

        // Find and invalidate token
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

            crow::json::wvalue response;
            response["status"] = "logged_out";
            response["message"] = "Logout successful";
            return crow::response(200, response);
        }

        crow::json::wvalue error;
        error["error"] = "Invalid token";
        error["message"] = "Token not found or already expired";
        return crow::response(404, error);
    }

    bool AuthHandler::ValidateToken(const std::string& token)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_tokens.find(token) != m_tokens.end();
    }
}
