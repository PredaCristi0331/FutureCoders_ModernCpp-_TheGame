#include "GameClient.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

GameClient::GameClient(const std::string& serverUrl, QObject* parent) 
    : QObject(parent), m_network(serverUrl) {}

bool GameClient::Login(const std::string& username) {
    json regPayload = {{"username", username}};
    m_network.Post("/auth/register", regPayload); 

    json payload = {{"username", username}};
    auto response = m_network.Post("/auth/login", payload);
    
    if (response.status_code == 200) {
        auto data = json::parse(response.text);
        m_username = username;
        m_userId = 1; 
        
        std::cout << "Login successful!" << std::endl;
        return true;
    } else {
        std::cout << "Login failed: " << response.text << std::endl;
        return false;
    }
}

bool GameClient::JoinGame(int gameId) {
    std::string playerName = m_username.empty() ? "Player" : m_username;
    json joinPayload = {{"playerName", playerName}};
    
    std::string endpoint = "/game/" + std::to_string(gameId) + "/join";
    auto response = m_network.Post(endpoint, joinPayload);

    if (response.status_code == 200) {
        auto data = json::parse(response.text);
        m_gameId = gameId; // Use the requested ID
        m_userId = data["playerIndex"].get<int>();
        m_isInGame = true;
        emit gameJoined(m_gameId);
        std::cout << "Joined Game " << m_gameId << " successfully." << std::endl;
        return true;
    } 
    
    std::cout << "Failed to join Game " << gameId << ": " << response.text << std::endl;
    return false;
}

bool GameClient::CreateGame(int maxPlayers) {
    json createPayload = {{"maxPlayers", maxPlayers}};
    auto createResp = m_network.Post("/game/create", createPayload);
    
    if (createResp.status_code == 200) {
        auto data = json::parse(createResp.text);
        int newGameId = data["gameId"];
        std::cout << "Created Game " << newGameId << std::endl;
        
        // Auto-join the created game
        return JoinGame(newGameId);
    }
    std::cout << "Failed to create game: " << createResp.text << std::endl;
    return false;
}

bool GameClient::JoinAnyGame() {
    // Try to find an existing game (e.g., game 0, 1, 2...)
    // For now, let's try Game 0. If 404, we create one.
    if(JoinGame(0)) return true;
    
    // If failed, create new
    return CreateGame(4);
}

void GameClient::PlayCard(int cardValue, int pileIndex) {
    if (!m_isInGame) return;

    json payload = {
        {"playerIndex", m_userId},
        {"cardValue", cardValue},
        {"pileIndex", pileIndex}
    };
    
    std::string endpoint = "/game/" + std::to_string(m_gameId) + "/play";
    auto response = m_network.Post(endpoint, payload);
    
    if(response.status_code == 200) {
        std::cout << "Move accepted!" << std::endl;
        PollGameState();
    } else {
        std::cout << "Move failed: " << response.text << std::endl;
    }
}

void GameClient::EndTurn() {
    if (!m_isInGame) return;

    json payload = {
        {"playerIndex", m_userId}
    };
    
    std::string endpoint = "/game/" + std::to_string(m_gameId) + "/endturn";
    auto response = m_network.Post(endpoint, payload);
    
    if(response.status_code == 200) {
        std::cout << "Turn ended!" << std::endl;
         PollGameState();
    } else {
        std::cout << "End turn failed: " << response.text << std::endl;
    }
}

void GameClient::DrawCards() {
    std::cout << "DrawCards is handled automatically at EndTurn." << std::endl;
}

void GameClient::SendChat(const std::string& message) {
    if (!m_isInGame) return;

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

    std::string endpoint = "/game/" + std::to_string(m_gameId) + "/state?userId=" + std::to_string(m_userId);
    auto response = m_network.Get(endpoint);
    
    if (response.status_code == 200) {
        try {
            auto data = json::parse(response.text);
            
            int curIdx = data.value("currentPlayerIndex", -1);
            m_currentState.isMyTurn = (curIdx == m_userId);
            
            m_currentState.piles.clear();
            auto piles = data["piles"];
            m_currentState.piles.push_back({true, piles.value("inc1", 1)});
            m_currentState.piles.push_back({true, piles.value("inc2", 1)});
            m_currentState.piles.push_back({false, piles.value("dec1", 100)});
            m_currentState.piles.push_back({false, piles.value("dec2", 100)});
            
            m_currentState.hand.clear();
            if(data.contains("myHand")) {
                for(const auto& c : data["myHand"]) {
                    m_currentState.hand.push_back({c["value"].get<int>()});
                }
            }
            
            m_currentState.otherPlayers.clear();
            if(data.contains("players")) {
                int idx = 0;
                for(const auto& p : data["players"]) {
                    if(idx != m_userId) {
                        PlayerInfo pi;
                        pi.id = idx;
                        pi.name = p.value("name", "Unknown");
                        pi.cardCount = p.value("cardCount", 0);
                        m_currentState.otherPlayers.push_back(pi);
                    }
                    idx++;
                }
            }
            
            emit gameStateUpdated(m_currentState);
            return true;
        } catch (const std::exception& e) {
             std::cout << "Error parsing game state: " << e.what() << std::endl;
        }
    }
    return false;
}
