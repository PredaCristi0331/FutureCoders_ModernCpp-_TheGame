#include "GameClient.h"
#include <iostream>

GameClient::GameClient(const std::string& serverUrl, QObject* parent) 
    : QObject(parent), m_network(serverUrl) {}

bool GameClient::Login(const std::string& username) {
    // Server expects: { "username": "..." }
    // Try Register first to ensure user exists
    json regPayload = {{"username", username}};
    m_network.Post("/auth/register", regPayload); 

    json payload = {{"username", username}};
    auto response = m_network.Post("/auth/login", payload);
    
    if (response.status_code == 200) {
        auto data = json::parse(response.text);
        // Server returns: { "status": "logged_in", "token": "...", "username": "..." }
        m_userId = 1; // Mock ID, token handled internally implicitly if needed later
        m_username = username;
        std::cout << "Login successful!" << std::endl;
        return true;
    } else {
        std::cout << "Login failed: " << response.text << std::endl;
        return false;
    }
}

bool GameClient::JoinGame() {
    // Logic: Try to Join Game 0. If 404, Create Game.
    // Server Join: POST /game/<id>/join, Body: { "playerName": ... }
    
    std::string playerName = m_username.empty() ? "Player" : m_username;
    json joinPayload = {{"playerName", playerName}};
    
    // 1. Try Join Game 0
    auto response = m_network.Post("/game/0/join", joinPayload);

    if (response.status_code == 200) {
        std::cout << "Joined Game 0!" << std::endl;
        m_gameId = 0;
        m_isInGame = true;
        emit gameJoined(m_gameId);
        return true;
    } 
    else if (response.status_code == 404 || response.status_code == 400 || response.status_code == 500) {
        // Game doesn't exist or is full? Let's try to Create one.
        std::cout << "Join failed (" << response.status_code << "). Creating new game..." << std::endl;
        
        json createPayload = {{"maxPlayers", 2}}; // Default to 2 players
        auto createResp = m_network.Post("/game/create", createPayload);
        
        if (createResp.status_code == 200) {
            auto data = json::parse(createResp.text);
            int newGameId = data["gameId"];
            std::cout << "Created Game " << newGameId << ". Joining..." << std::endl;
            
            // Re-try Join
            std::string endpoint = "/game/" + std::to_string(newGameId) + "/join";
            auto finalResp = m_network.Post(endpoint, joinPayload);
            
            if (finalResp.status_code == 200) {
                 m_gameId = newGameId;
                 m_isInGame = true;
                 emit gameJoined(m_gameId);
                 return true;
            }
        }
    }
    
    std::cout << "Failed to find or create a game." << std::endl;
    return false;
}

void GameClient::PlayCard(int cardValue, int pileIndex) {
    // Server doesn't support play_card yet.
    std::cout << "Server: PlayCard not implemented yet." << std::endl;
}

void GameClient::DrawCards() {
    // Server doesn't support draw_cards yet.
    std::cout << "Server: DrawCards not implemented yet." << std::endl;
}

void GameClient::SendChat(const std::string& message) {
    if (!m_isInGame) return;

    // Server: POST /chat, Body: { "gameId": ..., "playerName": ..., "message": ... }
    json payload = {
        {"gameId", m_gameId},
        {"playerName", m_username},
        {"message", message}
    };
    m_network.Post("/chat", payload);
}

GameState GameClient::GetGameState() {
    return m_currentState;
}

bool GameClient::PollGameState() {
    if (!m_isInGame) return false;

    // Server: GET /game/<id> -> Info (players, status)
    std::string endpoint = "/game/" + std::to_string(m_gameId);
    auto response = m_network.Get(endpoint);
    
    if (response.status_code == 200) {
        // We can parse status and players
        // For now, minimal support
        return true;
    }
    return false;
}
