#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSet>
#include <QString>

class GameClient;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(GameClient* client, QWidget* parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccessful(const QString& username);

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    QLineEdit* usernameInput;
    QPushButton* loginButton;
    QPushButton* registerButton;
    QLabel* messageLabel;
    QLabel* titleLabel;

    // Date mock pentru testare
    static QSet<QString> mockUsers;
    static void initializeMockUsers();

    // Validări
    bool validateUsername(const QString& username);
    bool mockLogin(const QString& username);
    bool mockRegister(const QString& username);
    void sendRequest(const QString& endpoint);
    GameClient* gameClient;
    
    // Design
    void setupUI();
    void applyStyles();
    
    QLineEdit* passwordInput;
};
