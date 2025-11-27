#pragma once
#include <string>
#include <vector>
#include "NetworkClient.h"
#include "GameState.h"

class GameClient {
public:
    explicit GameClient(const std::string& serverUrl);

    bool Login(const std::string& username);
    bool Register(const std::string& username, const std::string& password = "");
    bool JoinGame();
    void PlayCard(int cardValue, int pileIndex);
    void DrawCards();
    void SendChat(const std::string& message);
    GameState GetGameState();
    bool PollGameState(); // Returns true if state changed

    bool IsInGame() const { return m_isInGame; }
    int GetUserId() const { return m_userId; }

private:
    NetworkClient m_network;
    int m_userId = -1;
    int m_gameId = -1;
    bool m_isInGame = false;
    std::string m_username;
    GameState m_currentState;
};
