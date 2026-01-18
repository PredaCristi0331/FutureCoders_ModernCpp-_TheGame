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
    void onJoinGameClicked();
    void onCreateGameClicked();
    void onProfileClicked();
    void onRulesClicked();
    void onLogoutClicked();
    void onDifficultyChanged(int index);
    void onMatchmakingTimeout();
    
    void toggleSettings();
    void onThemeChanged(int index);

private:
    GameClient* gameClient;
    QString currentUsername;
    QString selectedDifficulty;

    QLabel* welcomeLabel;
    QLabel* difficultyLabel;
    QComboBox* difficultyComboBox;
    
    QLabel* numPlayersLabel;
    QComboBox* numPlayersComboBox;

    QPushButton* quickMatchButton;
    QPushButton* createGameButton;
    QPushButton* joinIdButton;
    
    QPushButton* profileButton;
    QPushButton* rulesButton;
    QPushButton* logoutButton;
    QLabel* statusLabel;

    QWidget* settingsOverlay{nullptr};
    QCheckBox* soundToggle{nullptr};
    QCheckBox* musicToggle{nullptr};
    QComboBox* themeCombo{nullptr};
    QPushButton* closeSettingsBtn{nullptr};
    QPushButton* settingsButton{nullptr};

    QWidget* waitingOverlay{nullptr};
    QLabel* waitingStatusLabel{nullptr};
    QLabel* gameIdDisplayLabel{nullptr};
    QLabel* playerCountLabel{nullptr};
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
};
