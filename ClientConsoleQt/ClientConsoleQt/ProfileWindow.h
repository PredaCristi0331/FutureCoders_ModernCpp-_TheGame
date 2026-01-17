#pragma once

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

class GameClient; // Forward declaration

class ProfileWindow : public QWidget {
    Q_OBJECT

public:
    explicit ProfileWindow(GameClient* client, QWidget* parent = nullptr);
    ~ProfileWindow() override;

    void setUsername(const QString& username);
    void refreshStats(); // New method

signals:
    void backToLobby();

private slots:
    void onBackClicked();

protected:
    void showEvent(QShowEvent* event) override;

private:
    GameClient* m_client;
    QString currentUsername;
    
    // UI Elements
    QLabel* titleLabel{nullptr};
    QLabel* usernameLabel{nullptr};
    QLabel* statsLabel{nullptr}; // Mock stats
    QPushButton* backButton{nullptr};
    
    void setupUI();
    void applyStyles();
    void connectSignals();
};
