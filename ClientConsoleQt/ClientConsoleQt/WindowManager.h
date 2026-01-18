#pragma once

#include <QStackedWidget>
#include <QString>


class LoginWindow;
class LobbyWindow;
class ProfileWindow;
class GameBoardWindow;
class GameClient;


class WindowManager : public QStackedWidget {
    Q_OBJECT

public:
    enum WindowType {
        LOGIN = 0,
        LOBBY = 1,
        PROFILE = 2,
        GAME_BOARD = 3,

    };

    explicit WindowManager(QWidget* parent = nullptr);
    ~WindowManager();


    void showLoginWindow();
    void showLobbyWindow(const QString& username);
    void showProfileWindow(const QString& username);
    void showGameBoardWindow(const QString& username);
    


signals:
    void windowChanged(WindowType newWindow);

private:
    void initWindows();


    LoginWindow* loginWindow;
    LobbyWindow* lobbyWindow;
    ProfileWindow* profileWindow;
    GameBoardWindow* gameBoardWindow;
    


    GameClient* gameClient;
};
