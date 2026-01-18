#include "WindowManager.h"
#include "LoginWindow.h"
#include "LobbyWindow.h"
#include "ProfileWindow.h"

#include "GameClient.h"
#include "GameBoardWindow.h"

WindowManager::WindowManager(QWidget* parent)
    : QStackedWidget(parent)
    , loginWindow(nullptr)
    , lobbyWindow(nullptr)
    , profileWindow(nullptr)
    , gameBoardWindow(nullptr)
{
    gameClient = new GameClient("http://localhost:18080", this);
    initWindows();
}

WindowManager::~WindowManager() {
}

void WindowManager::initWindows() {
    loginWindow = new LoginWindow(gameClient, this);
    addWidget(loginWindow);
    connect(loginWindow, &LoginWindow::loginSuccessful, 
            this, &WindowManager::showLobbyWindow);

    lobbyWindow = new LobbyWindow(gameClient, this);
    addWidget(lobbyWindow);
    connect(lobbyWindow, &LobbyWindow::startGame, 
            this, &WindowManager::showGameBoardWindow);
    connect(lobbyWindow, &LobbyWindow::showProfile,
            this, &WindowManager::showProfileWindow);
    connect(lobbyWindow, &LobbyWindow::logout,
            this, &WindowManager::showLoginWindow);

    profileWindow = new ProfileWindow(gameClient, this);
    addWidget(profileWindow);
    connect(profileWindow, &ProfileWindow::backToLobby, 
            [this]() { showLobbyWindow(""); }); 
    
    gameBoardWindow = new GameBoardWindow(gameClient, this);
    addWidget(gameBoardWindow);
    connect(gameBoardWindow, &GameBoardWindow::backToLobby,
            [this]() { 
                if (gameClient) {
                    gameClient->LeaveGame();
                }
                showLobbyWindow(""); 
            });
}

void WindowManager::showLoginWindow() {
    setCurrentIndex(LOGIN);
    emit windowChanged(LOGIN);
}

void WindowManager::showLobbyWindow(const QString& username) {
    if (lobbyWindow) {
        if (!username.isEmpty()) {
            lobbyWindow->setUsername(username);
        }
        setCurrentIndex(LOBBY);
        emit windowChanged(LOBBY);
    }
}

void WindowManager::showProfileWindow(const QString& username) {
    if (profileWindow) {
        profileWindow->setUsername(username);
        setCurrentIndex(PROFILE);
        emit windowChanged(PROFILE);
    }
}

void WindowManager::showGameBoardWindow(const QString& username) {
    if (gameBoardWindow) {
        gameBoardWindow->setUsername(username);
        setCurrentIndex(GAME_BOARD);
        emit windowChanged(GAME_BOARD);
    }
}
