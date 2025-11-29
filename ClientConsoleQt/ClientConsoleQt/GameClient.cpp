#include "GameClient.h"

GameClient::GameClient(const std::string& serverUrl, QObject* parent) 
    : QObject(parent), m_network(serverUrl) {}

bool GameClient::Login(const std::string& username) {
    json payload = {{"username", username}};
    auto response = m_network.Post("/api/login", payload);
    
    if (response.status_code == 200) {
        auto data = json::parse(response.text);
        m_userId = data["user_id"];
        m_username = username;
        std::cout << "Login successful! User ID: " << m_userId << std::endl;
        return true;
    } else {
        std::cout << "Login failed: " << response.text << std::endl;
        return false;
    }
}

bool GameClient::JoinGame() {
    if (m_userId == -1) return false;

    json payload = {{"user_id", m_userId}};
    auto response = m_network.Post("/api/join_game", payload);

    if (response.status_code == 200) {
        auto data = json::parse(response.text);
        m_gameId = data["game_id"];
        m_isInGame = true;
        std::cout << "Joined game! Game ID: " << m_gameId << std::endl;
        return true;
    }
    return false;
}

void GameClient::PlayCard(int cardValue, int pileIndex) {
    if (!m_isInGame) return;
    
    json payload = {
        {"game_id", m_gameId},
        {"user_id", m_userId},
        {"card_value", cardValue},
        {"pile_index", pileIndex}
    };
    m_network.Post("/api/play_card", payload);
}

void GameClient::DrawCards() {
    if (!m_isInGame) return;
    
    json payload = {
        {"game_id", m_gameId},
        {"user_id", m_userId}
    };
    m_network.Post("/api/draw_cards", payload);
}

void GameClient::SendChat(const std::string& message) {
    if (!m_isInGame) return;

    json payload = {
        {"game_id", m_gameId},
        {"user_id", m_userId},
        {"message", message}
    };
    m_network.Post("/api/chat", payload);
}

GameState GameClient::GetGameState() {
    return m_currentState;
}

bool GameClient::PollGameState() {
    if (!m_isInGame) return false;

    std::string endpoint = "/api/game_state/" + std::to_string(m_gameId);
    auto response = m_network.Get(endpoint);
    
    if (response.status_code == 200) {
        try {
            auto data = json::parse(response.text);
            
            m_currentState.gameId = m_gameId;
            m_currentState.deckSize = data.value("deck_size", 0);
            m_currentState.isMyTurn = (data.value("current_player_id", -1) == m_userId);
            
            // Parse Piles
            m_currentState.piles.clear();
            if (data.contains("piles")) {
                for (const auto& p : data["piles"]) {
                    m_currentState.piles.push_back({
                        p.value("is_ascending", true),
                        p.value("top_card", 0)
                    });
                }
            }

            // Parse Hand
            m_currentState.hand.clear();
            if (data.contains("hand")) {
                for (const auto& c : data["hand"]) {
                    m_currentState.hand.push_back({c.value("value", 0)});
                }
            }

            // Parse Chat
            m_currentState.chatMessages.clear();
            if (data.contains("chat")) {
                for (const auto& msg : data["chat"]) {
                    m_currentState.chatMessages.push_back(msg.get<std::string>());
                }
            }
            
            return true;
        } catch (...) {
            std::cerr << "Error parsing game state." << std::endl;
        }
    }
    return false;
}
