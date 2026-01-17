#include "GameClient.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

GameClient::GameClient(const std::string& serverUrl, QObject* parent) 
    : QObject(parent), m_network(serverUrl) {}

bool GameClient::Login(const std::string& username, const std::string& password) {
    json payload = {
        {"username", username},
        {"password", password}
    };
    auto response = m_network.Post("/auth/login", payload);
    
    if (response.status_code == 200) {
        try {
            auto data = json::parse(response.text);
            m_username = username;
            if (data.contains("userId")) {
                m_userId = data["userId"].get<int>();
            } else {
                m_userId = 1; // Fallback? Or fail?
            }
            
            std::cout << "Login successful! UserID: " << m_userId << std::endl;
            return true;
        } catch (...) {
            std::cout << "Login error: Invalid response format" << std::endl;
            return false;
        }
    } else {
        std::cout << "Login failed: " << response.text << std::endl;
        return false;
    }
}

// ... (skipping Register implementation from previous edit as it's separate)

// ...


bool GameClient::Register(const std::string& username, const std::string& password) {
    json regPayload = {
        {"username", username},
        {"password", password}
    };
    auto response = m_network.Post("/auth/register", regPayload); 
    
    if (response.status_code == 200) {
        std::cout << "Registration successful!" << std::endl;
        return true;
    } else {
        std::cout << "Registration failed: " << response.text << std::endl;
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
        m_playerIndex = data["playerIndex"].get<int>();
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
        {"playerIndex", m_playerIndex},
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
        {"playerIndex", m_playerIndex}
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
        {"playerId", m_userId},
        {"text", message}
    };
    m_network.Post("/chat", payload);
}

void GameClient::PollChat() {
    if (!m_isInGame) return;

    std::string endpoint = "/chat/" + std::to_string(m_gameId);
    auto response = m_network.Get(endpoint);

    if (response.status_code == 200) {
        try {
            auto data = json::parse(response.text);
            std::vector<ChatMessage> messages;
            
            if (data.is_array()) {
                for (const auto& item : data) {
                    ChatMessage msg;
                    msg.id = item.value("id", 0);
                    msg.playerId = item.value("playerId", 0);
                    msg.gameId = item.value("gameId", 0);
                    msg.text = item.value("text", "");
                    msg.timestamp = item.value("timestamp", "");
                    messages.push_back(msg);
                }
            }
            emit chatUpdated(messages);
        } catch (...) {
            std::cout << "Error parsing chat" << std::endl;
        }
    }
}

GameState GameClient::GetGameState() {
    return m_currentState;
}

bool GameClient::PollGameState() {
    if (!m_isInGame) return false;

    std::string endpoint = "/game/" + std::to_string(m_gameId) + "/state?userId=" + std::to_string(m_playerIndex);
    auto response = m_network.Get(endpoint);

    if (response.status_code == 200) {
        try {
            auto data = json::parse(response.text);

            m_currentState.status = data.value("status", "unknown");
            m_currentState.currentPlayers = data.value("currentPlayers", 0);
            m_currentState.maxPlayers = data.value("maxPlayers", 0);
            m_currentState.deckSize = data.value("deckCount", 98); // Parse deckCount to deckSize

            int curIdx = data.value("currentPlayerIndex", -1);
            m_currentState.isMyTurn = (curIdx == m_playerIndex);
            m_currentState.currentPlayerName = data.value("currentPlayerName", "Unknown");

            m_currentState.piles.clear();
            auto piles = data["piles"];
            m_currentState.piles.push_back({ true, piles.value("inc1", 1) });
            m_currentState.piles.push_back({ true, piles.value("inc2", 1) });
            m_currentState.piles.push_back({ false, piles.value("dec1", 100) });
            m_currentState.piles.push_back({ false, piles.value("dec2", 100) });

            m_currentState.hand.clear();
            if (data.contains("myHand")) {
                for (const auto& c : data["myHand"]) {
                    m_currentState.hand.push_back({ c["value"].get<int>() });
                }
            }

            m_currentState.otherPlayers.clear();
            if (data.contains("players")) {
                int idx = 0;
                for (const auto& p : data["players"]) {
                    if (idx != m_playerIndex) {
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
        }
        catch (const std::exception& e) {
            std::cout << "Error parsing game state: " << e.what() << std::endl;
        }
        return false;
    }
}

std::optional<GameClient::UserProfile> GameClient::GetUserProfile(int userId) {
    if (userId < 0) return std::nullopt;

    std::string endpoint = "/user/" + std::to_string(userId) + "/profile";
    auto response = m_network.Get(endpoint);

    if (response.status_code == 200) {
        try {
            auto data = json::parse(response.text);
            UserProfile p;
            p.username = data.value("username", "Unknown");
            p.games_played = data.value("games_played", 0);
            p.games_won = data.value("games_won", 0);
            p.games_lost = data.value("games_lost", 0);
            p.performance_score = data.value("performance_score", 1);
            p.hours_played = data.value("hours_played", 0.0);
            return p;
        } catch (...) {
            std::cout << "Error parsing profile" << std::endl;
        }
    }
    return std::nullopt;
}
