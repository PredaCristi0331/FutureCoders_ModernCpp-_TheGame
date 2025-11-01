#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onServerResponse(QNetworkReply* reply);

private:
    QLineEdit* usernameInput;
    QPushButton* loginButton;
    QPushButton* registerButton;
    QLabel* messageLabel;
    QNetworkAccessManager* networkManager;

    void sendRequest(const QString& endpoint);
};

#endif // LOGINWINDOW_H
