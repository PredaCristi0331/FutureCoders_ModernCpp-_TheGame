#pragma once

#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QFrame>
#include <QCheckBox>
#include <QProgressBar>

class GameClient;

/**
 * LobbyWindow - Fereastra de lobby cu selecție dificultate
 * 
 * Permite utilizatorului să:
 * - Selecteze nivelul de dificultate (Ușor/Mediu/Greu)
 * - Caută un joc sau pornește un joc nou
 * - Vezi statusul de matchmaking
 */
class LobbyWindow : public QWidget {
    Q_OBJECT

public:
    explicit LobbyWindow(GameClient* client, QWidget* parent = nullptr);
    ~LobbyWindow();

    void setUsername(const QString& username);

signals:
    void startGame(const QString& username, const QString& difficulty);
    void showProfile(const QString& username);
    void logout();

private slots:
    void onPlayClicked();
    void onProfileClicked();
    void onRulesClicked();
    void onLogoutClicked();
    void onDifficultyChanged(int index);
    void onMatchmakingTimeout();
    // Inline Slots
    void toggleSettings();
    void onThemeChanged(int index);

private:
    GameClient* gameClient;
    QString currentUsername;
    QString selectedDifficulty;

    // Widget-uri UI
    QLabel* welcomeLabel;
    QLabel* difficultyLabel;
    QComboBox* difficultyComboBox;
    QPushButton* playButton;
    QPushButton* profileButton;
    QPushButton* rulesButton;
    QPushButton* logoutButton;
    QLabel* statusLabel;

    // Settings UI (Inline Commit 1)
    QWidget* settingsOverlay{nullptr};
    QCheckBox* soundToggle{nullptr};
    QCheckBox* musicToggle{nullptr};
    QComboBox* themeCombo{nullptr};
    QPushButton* closeSettingsBtn{nullptr};
    QPushButton* settingsButton{nullptr}; // New button in main UI

    // Waiting UI (Inline Commit 2)
    QWidget* waitingOverlay{nullptr};
    QLabel* waitingStatusLabel{nullptr};
    QProgressBar* waitingProgress{nullptr};
    QPushButton* cancelWaitBtn{nullptr};
    QTimer* matchmakingTimer{nullptr};

    void setupSettingsUI();
    void setupWaitingUI();
    void showWaitingScreen();
    void hideWaitingScreen();

    void setupUI();
    void applyStyles();
    void connectSignals();
    void updateStatus(const QString& message, const QString& color);
};//commit

