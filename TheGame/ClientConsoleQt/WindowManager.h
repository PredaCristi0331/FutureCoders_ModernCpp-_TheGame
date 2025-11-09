#pragma once

#include <QStackedWidget>
#include <QString>

// Forward declarations pentru toate ferestrele
class LoginWindow;
class LobbyWindow;
class ProfileWindow;
class GameBoardWindow;

/**
 * WindowManager - Gestionează navigarea între ferestre
 * 
 * Această clasă permite adăugarea ușoară de ferestre noi:
 * 1. Adaugă forward declaration în header
 * 2. Adaugă pointer în members
 * 3. Creează fereastra în initWindows()
 * 4. Adaugă metoda de navigare (ex: showLobbyWindow())
 * 5. Adaugă index-ul în enum WindowType
 */
class WindowManager : public QStackedWidget {
    Q_OBJECT

public:
    enum WindowType {
        LOGIN = 0,
        LOBBY = 1,
        PROFILE = 2,
        GAME_BOARD = 3,
        // TODO: Adaugă aici noi tipuri de ferestre pentru commit-urile viitoare
        // SETTINGS = 4,
        // STATISTICS = 5,
        // etc.
    };

    explicit WindowManager(QWidget* parent = nullptr);
    ~WindowManager();

    // Metode de navigare - adaugă metode noi pentru fiecare fereastră
    void showLoginWindow();
    void showLobbyWindow(const QString& username);
    void showProfileWindow(const QString& username);
    void showGameBoardWindow(const QString& username);
    
    // TODO: Adaugă aici metode noi pentru ferestre viitoare:
    // void showSettingsWindow();
    // void showStatisticsWindow();

signals:
    void windowChanged(WindowType newWindow);

private:
    void initWindows();

    // Pointers către ferestre - adaugă pointer nou pentru fiecare fereastră
    LoginWindow* loginWindow;
    LobbyWindow* lobbyWindow;
    ProfileWindow* profileWindow;
    GameBoardWindow* gameBoardWindow;
    
    // TODO: Adaugă aici pointeri noi pentru ferestre viitoare:
    // SettingsWindow* settingsWindow;
    // StatisticsWindow* statisticsWindow;
};

