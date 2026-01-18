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
        auto body = crow::json::load(req.body);
        if (!body)
        {
            crow::json::wvalue error;
            error["error"] = "Invalid JSON format";
            error["message"] = "Request body must be valid JSON";
            return crow::response(400, error);
        }

        if (!body.has("username") || !body.has("password"))
        {
            crow::json::wvalue error;
            error["error"] = "Missing field";
            error["message"] = "Username and password fields are required";
            return crow::response(400, error);
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        if (username.length() < 3)
        {
            crow::json::wvalue error;
            error["error"] = "Invalid username";
            error["message"] = "Username must be at least 3 characters long";
            return crow::response(400, error);
        }

        bool success = DatabaseManager::registerUser(username, password);
        if (!success)
        {
             crow::json::wvalue error;
             error["error"] = "Registration failed";
             error["message"] = "User already exists or username invalid.";
             return crow::response(409, error);
        }

        
        std::string token = GenerateToken(username);
        
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_tokens[token] = username;
        }

        crow::json::wvalue response;
        response["username"] = username;
        response["token"] = token;
        response["status"] = "registered";
        response["message"] = "User registered successfully";

        return crow::response(200, response);
    }

    crow::response AuthHandler::Login(const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");
        if (!body.has("username") || !body.has("password")) return crow::response(400, "Missing credentials");

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        auto userOpt = DatabaseManager::loginUser(username, password);
        if (!userOpt)
        {
            crow::json::wvalue error;
            error["error"] = "Authentication failed";
            error["message"] = "Invalid username or password.";
            return crow::response(401, error);
        }

        std::string token = GenerateToken(username);

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            m_tokens[token] = username;
        }

        crow::json::wvalue response;
        response["username"] = username;
        response["token"] = token;
        response["userId"] = userOpt->id;
        response["status"] = "logged_in";
        response["message"] = "Login successful";

        return crow::response(200, response);
    }

    crow::response AuthHandler::Logout(const crow::request& req)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto body = crow::json::load(req.body);
        if (!body || !body.has("token")) return crow::response(400, "Missing token");

        std::string token = body["token"].s();
        if (m_tokens.erase(token))
        {
            crow::json::wvalue response;
            response["status"] = "logged_out";
            return crow::response(200, response);
        }
        return crow::response(404, "Token not found");
    }

    bool AuthHandler::ValidateToken(const std::string& token)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_tokens.find(token) != m_tokens.end();
    }

    crow::response AuthHandler::GetProfile(const std::string& token)
    {
        std::string username;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_tokens.find(token);
            if (it == m_tokens.end()) return crow::response(401, "Invalid token");
            username = it->second;
        }

        if(username.empty()) return crow::response(401, "Invalid token");

        auto userOpt = DatabaseManager::getUserByUsername(username);
        if(!userOpt) return crow::response(404, "User not found");
        
        UserProfile profile = DatabaseManager::getUserProfile(userOpt->id);
        
        crow::json::wvalue response;
        response["username"] = profile.username;
        response["games_played"] = profile.games_played;
        response["games_won"] = profile.games_won;
        response["games_lost"] = profile.games_lost;
        response["performance_score"] = profile.performance_score;
        
        response["hours_played"] = static_cast<double>(profile.hours_played_seconds) / 3600.0;
        
        return crow::response(200, response);
    }
}
