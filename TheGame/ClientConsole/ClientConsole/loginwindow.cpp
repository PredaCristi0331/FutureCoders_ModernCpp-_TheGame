#include "loginwindow.h"
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QUrl>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("The Game - Login");
    resize(300, 200);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Introdu numele de utilizator");

    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    messageLabel = new QLabel(this);
    messageLabel->setStyleSheet("color: red;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(usernameInput);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    layout->addWidget(messageLabel);

    networkManager = new QNetworkAccessManager(this);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &LoginWindow::onServerResponse);
}

void LoginWindow::onLoginClicked() {
    sendRequest("login");
}

void LoginWindow::onRegisterClicked() {
    sendRequest("register");
}

void LoginWindow::sendRequest(const QString& endpoint) {
    QString username = usernameInput->text().trimmed();
    if (username.isEmpty()) {
        messageLabel->setText("Introduceți un nume de utilizator!");
        return;
    }

    QUrl url("http://127.0.0.1:18080/" + endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    QByteArray data = QJsonDocument(json).toJson();

    networkManager->post(request, data);
}

void LoginWindow::onServerResponse(QNetworkReply* reply) {
    QByteArray response = reply->readAll();
    if (reply->error() != QNetworkReply::NoError) {
        messageLabel->setText("Eroare de conexiune la server.");
    }
    else {
        QString res = QString(response);
        if (res.contains("success")) {
            QMessageBox::information(this, "Succes", "Autentificare reușită!");
        }
        else {
            messageLabel->setText("Autentificare eșuată!");
        }
    }
    reply->deleteLater();
}
