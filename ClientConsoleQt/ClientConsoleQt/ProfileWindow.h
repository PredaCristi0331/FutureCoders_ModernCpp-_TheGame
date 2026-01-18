#pragma once

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

class GameClient;

class ProfileWindow : public QWidget {
    Q_OBJECT

public:
    explicit ProfileWindow(GameClient* client, QWidget* parent = nullptr);
    ~ProfileWindow() override;

    void setUsername(const QString& username);
    void refreshStats();

signals:
    void backToLobby();

private slots:
    void onBackClicked();

protected:
    void showEvent(QShowEvent* event) override;

private:
    GameClient* m_client;
    QString currentUsername;
    
    QLabel* titleLabel{nullptr};
    QLabel* usernameLabel{nullptr};
    QLabel* statsLabel{nullptr};
    QPushButton* backButton{nullptr};
    
    void setupUI();
    void applyStyles();
    void connectSignals();
};
