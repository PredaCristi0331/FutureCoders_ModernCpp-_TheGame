#pragma once
#include <QObject>
#include "NetworkClient.h"
#include "GameState.h"
#include <string>
#include <vector>
#include <iostream>
#include <optional>

class GameClient : public QObject {
    Q_OBJECT
public:
    explicit GameClient(const std::string& serverUrl, QObject* parent = nullptr);

    bool Login(const std::string& username, const std::string& password);
    bool Register(const std::string& username, const std::string& password);
    bool JoinGame(int gameId);
    bool CreateGame(int maxPlayers);
    bool JoinAnyGame();
    void PlayCard(int cardValue, int pileIndex);
    void EndTurn(); // New method
    void DrawCards();
    void SendChat(const std::string& message);
    GameState GetGameState();
    bool PollGameState(); // Returns true if state changed

    struct ChatMessage {
        int id;
        int playerId;
        int gameId;
        std::string text;
        std::string timestamp;
    };
    void PollChat();

    struct UserProfile {
        std::string username;
        int games_played;
        int games_won;
        int games_lost;
        int performance_score;
        int total_minutes_played;
    };
    std::optional<UserProfile> GetUserProfile(int userId);

    bool IsInGame() const { return m_isInGame; }
    int GetUserId() const { return m_userId; }
    int GetGameId() const { return m_gameId; }
    void LeaveGame(); // Reset game state when leaving

signals:
    void loginSuccess(int userId);
    void loginFailed(const QString& reason);
    void gameJoined(int gameId);
    void gameStateUpdated(const GameState& state);
    void chatUpdated(const std::vector<ChatMessage>& messages);

private:
    NetworkClient m_network;
    int m_userId = -1;
    int m_playerIndex = -1; // Seat index in game (0-4)
    int m_gameId = -1;
    bool m_isInGame = false;
    std::string m_username;
    GameState m_currentState;
};
