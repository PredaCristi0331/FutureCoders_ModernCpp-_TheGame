#include <QObject>
#include "NetworkClient.h"
#include "GameState.h"
#include <string>
#include <vector>
#include <iostream>

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

    bool IsInGame() const { return m_isInGame; }
    int GetUserId() const { return m_userId; }
    int GetGameId() const { return m_gameId; }

signals:
    void loginSuccess(int userId);
    void loginFailed(const QString& reason);
    void gameJoined(int gameId);
    void gameStateUpdated(const GameState& state);

private:
    NetworkClient m_network;
    int m_userId = -1;
    int m_playerIndex = -1; // Seat index in game (0-4)
    int m_gameId = -1;
    bool m_isInGame = false;
    std::string m_username;
    GameState m_currentState;
};
