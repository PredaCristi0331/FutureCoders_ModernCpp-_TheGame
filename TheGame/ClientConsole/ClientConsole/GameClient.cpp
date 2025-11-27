#include "GameClient.h"
#include <iostream>
#include <thread>
#include <chrono>

GameClient::GameClient(const std::string& serverUrl)
    : m_network(serverUrl) {}

bool GameClient::Login(const std::string& username) {
    json payload;
    payload["username"] = username;

    auto response = m_network.Post("/api/auth/login", payload);

    if (response.status_code == 200) {
        auto body = json::parse(response.text);
        if (body["success"].get<bool>()) {
            m_userId = body["userId"].get<int>();
            m_username = body["username"].get<std::string>();
            std::cout << "[SUCCESS] Logged in as " << m_username << " (ID: " << m_userId << ")" << std::endl;
            return true;
        }
    }

    std::cerr << "[ERROR] Login failed: " << response.text << std::endl;
    return false;
}

bool GameClient::Register(const std::string& username, const std::string& password) {
    json payload;
    payload["username"] = username;
    if (!password.empty()) {
        payload["password"] = password;
    }

    auto response = m_network.Post("/api/auth/register", payload);

    if (response.status_code == 200) {
        auto body = json::parse(response.text);
        if (body["success"].get<bool>()) {
            m_userId = body["userId"].get<int>();
            m_username = body["username"].get<std::string>();
            std::cout << "[SUCCESS] Registered as " << m_username << " (ID: " << m_userId << ")" << std::endl;
            return true;
        }
    }

    std::cerr << "[ERROR] Registration failed: " << response.text << std::endl;
    return false;
}

bool GameClient::JoinGame() {
    json payload;
    payload["userId"] = m_userId;
    payload["username"] = m_username;

    auto response = m_network.Post("/api/matchmaking/join", payload);

    if (response.status_code != 200) {
        std::cerr << "[ERROR] Failed to join matchmaking" << std::endl;
        return false;
    }

    std::cout << "[INFO] Joined matchmaking queue. Waiting for players..." << std::endl;

    // Poll for match (max 35 seconds)
    for (int i = 0; i < 35; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::string url = "/api/matchmaking/status?userId=" + std::to_string(m_userId);
        auto statusResponse = m_network.Get(url);

        if (statusResponse.status_code == 200) {
            auto body = json::parse(statusResponse.text);
            std::string status = body["status"].get<std::string>();

            if (status == "matched") {
                m_gameId = body["gameId"].get<int>();
                m_isInGame = true;
                std::cout << "[SUCCESS] Game found! Game ID: " << m_gameId << std::endl;
                return true;
            }
        }

        std::cout << "[INFO] Waiting... (" << (i + 1) << "s)" << std::endl;
    }

    std::cout << "[TIMEOUT] No match found within 35 seconds" << std::endl;
    return false;
}

void GameClient::PlayCard(int cardValue, int pileIndex) {
    json payload;
    payload["userId"] = m_userId;
    payload["cardValue"] = cardValue;
    payload["pileIndex"] = pileIndex;

    std::string url = "/api/game/" + std::to_string(m_gameId) + "/play";
    auto response = m_network.Post(url, payload);

    if (response.status_code == 200) {
        std::cout << "[SUCCESS] Card " << cardValue << " played on pile " << pileIndex << std::endl;
    }
    else {
        std::cerr << "[ERROR] Failed to play card: " << response.text << std::endl;
    }
}

void GameClient::DrawCards() {
    // This is a placeholder - drawing cards happens automatically in the GameTable logic
    std::cout << "[INFO] Draw cards (handled by server automatically)" << std::endl;
}

void GameClient::SendChat(const std::string& message) {
    json payload;
    payload["playerName"] = m_username;
    payload["message"] = message;

    std::string url = "/api/game/" + std::to_string(m_gameId) + "/chat";
    auto response = m_network.Post(url, payload);

    if (response.status_code == 200) {
        std::cout << "[SENT] " << message << std::endl;
    }
    else {
        std::cerr << "[ERROR] Failed to send chat message" << std::endl;
    }
}

GameState GameClient::GetGameState() {
    return m_currentState;
}

bool GameClient::PollGameState() {
    if (!m_isInGame) return false;

    std::string url = "/api/game/" + std::to_string(m_gameId) + "/state?userId=" + std::to_string(m_userId);
    auto response = m_network.Get(url);

    if (response.status_code != 200) {
        std::cerr << "[ERROR] Failed to get game state" << std::endl;
        return false;
    }

    auto body = json::parse(response.text);
    if (!body["success"].get<bool>()) {
        return false;
    }

    auto data = body["data"];

    // Parse game state
    m_currentState.gameId = data["gameId"].get<int>();
    m_currentState.isMyTurn = data["isMyTurn"].get<bool>();
    m_currentState.deckSize = data["deckSize"].get<int>();

    // Parse piles
    m_currentState.piles.clear();
    for (const auto& pile : data["piles"]) {
        Pile p;
        p.isAscending = pile["isAscending"].get<bool>();
        p.topCardValue = pile["topCardValue"].get<int>();
        m_currentState.piles.push_back(p);
    }

    // Parse hand
    m_currentState.hand.clear();
    for (const auto& card : data["hand"]) {
        GameCard c;
        c.value = card["value"].get<int>();
        m_currentState.hand.push_back(c);
    }

    // Parse other players
    m_currentState.otherPlayers.clear();
    for (const auto& player : data["otherPlayers"]) {
        PlayerInfo p;
        p.id = player["id"].get<int>();
        p.name = player["name"].get<std::string>();
        p.cardCount = player["cardCount"].get<int>();
        m_currentState.otherPlayers.push_back(p);
    }

    // Parse chat messages
    m_currentState.chatMessages.clear();
    for (const auto& msg : data["chatMessages"]) {
        m_currentState.chatMessages.push_back(msg.get<std::string>());
    }

    // Check if game ended
    if (data.contains("gameEnded") && data["gameEnded"].get<bool>()) {
        m_isInGame = false;
        bool teamWon = data["teamWon"].get<bool>();
        std::cout << "\n========================================" << std::endl;
        if (teamWon) {
            std::cout << "  🎉 VICTORY! The team won! 🎉" << std::endl;
        }
        else {
            std::cout << "  ❌ DEFEAT! Better luck next time!" << std::endl;
        }
        std::cout << "========================================\n" << std::endl;
    }

    return true;
}
