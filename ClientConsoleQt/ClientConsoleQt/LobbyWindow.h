#pragma once

#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

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
    explicit LobbyWindow(QWidget* parent = nullptr);
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

private:
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
    QTimer* matchmakingTimer;

    void setupUI();
    void applyStyles();
    void connectSignals();
    void updateStatus(const QString& message, const QString& color);
};//commit

