#include "WindowManager.h"
#include "LoginWindow.h"
#include "LobbyWindow.h"
// TODO: Include aici ferestre noi când le creezi:
// #include "ProfileWindow.h"
// #include "GameBoardWindow.h"

#include "GameClient.h"

WindowManager::WindowManager(QWidget* parent)
    : QStackedWidget(parent)
    , loginWindow(nullptr)
    , lobbyWindow(nullptr)
    , profileWindow(nullptr)
    , gameBoardWindow(nullptr)
{
    // Initialize GameClient with default URL (can be configurable)
    gameClient = new GameClient("http://localhost:18080", this);
    initWindows();
}

WindowManager::~WindowManager() {
    // Widget-urile vor fi șterse automat de Qt
}

void WindowManager::initWindows() {
    // Creează LoginWindow
    loginWindow = new LoginWindow(gameClient, this);
    addWidget(loginWindow);
    connect(loginWindow, &LoginWindow::loginSuccessful, 
            this, &WindowManager::showLobbyWindow);

    // Creează LobbyWindow
    lobbyWindow = new LobbyWindow(this);
    addWidget(lobbyWindow);
    connect(lobbyWindow, &LobbyWindow::startGame, 
            this, &WindowManager::showGameBoardWindow);
    connect(lobbyWindow, &LobbyWindow::showProfile,
            this, &WindowManager::showProfileWindow);
    connect(lobbyWindow, &LobbyWindow::logout,
            this, &WindowManager::showLoginWindow);

    // TODO: Creează aici ferestre noi pentru commit-urile viitoare:
    // profileWindow = new ProfileWindow(this);
    // addWidget(profileWindow);
    // 
    // gameBoardWindow = new GameBoardWindow(this);
    // addWidget(gameBoardWindow);
}

void WindowManager::showLoginWindow() {
    setCurrentIndex(LOGIN);
    emit windowChanged(LOGIN);
}

void WindowManager::showLobbyWindow(const QString& username) {
    if (lobbyWindow) {
        lobbyWindow->setUsername(username);
        setCurrentIndex(LOBBY);
        emit windowChanged(LOBBY);
    }
}

void WindowManager::showProfileWindow(const QString& username) {
    // TODO: Implementează în commit-ul pentru ProfileWindow
    // if (!profileWindow) {
    //     profileWindow = new ProfileWindow(this);
    //     addWidget(profileWindow);
    // }
    // profileWindow->setUsername(username);
    // setCurrentIndex(PROFILE);
    // emit windowChanged(PROFILE);
}

void WindowManager::showGameBoardWindow(const QString& username) {
    // TODO: Implementează în commit-ul pentru GameBoardWindow
    // Pentru moment, afișăm un mesaj temporar
    // if (!gameBoardWindow) {
    //     gameBoardWindow = new GameBoardWindow(this);
    //     addWidget(gameBoardWindow);
    // }
    // gameBoardWindow->setUsername(username);
    // setCurrentIndex(GAME_BOARD);
    // emit windowChanged(GAME_BOARD);
    
    // Temporar: revenim la lobby (va fi înlocuit cu GameBoardWindow real)
    if (lobbyWindow) {
        lobbyWindow->setUsername(username);
        setCurrentIndex(LOBBY);
        emit windowChanged(LOBBY);
    }
}

