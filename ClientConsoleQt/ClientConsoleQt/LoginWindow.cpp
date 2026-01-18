#include "LoginWindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QFont>
#include <QPalette>

#include "GameClient.h"


LoginWindow::LoginWindow(GameClient* client, QWidget* parent) 
    : QWidget(parent), gameClient(client) {
    setupUI();
    applyStyles();
}

LoginWindow::~LoginWindow() {}



void LoginWindow::setupUI() {

    setMinimumSize(400, 300);


    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);


    titleLabel = new QLabel("THE GAME", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Arial", 28, QFont::Bold);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);


    mainLayout->addStretch();


    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Nume de utilizator");
    usernameInput->setMinimumHeight(40);
    mainLayout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Parola");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(40);
    mainLayout->addWidget(passwordInput);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    loginButton = new QPushButton("Login", this);
    loginButton->setMinimumHeight(40);
    loginButton->setMinimumWidth(120);

    registerButton = new QPushButton("Register", this);
    registerButton->setMinimumHeight(40);
    registerButton->setMinimumWidth(120);

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    mainLayout->addLayout(buttonLayout);


    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setWordWrap(true);
    messageLabel->setMinimumHeight(30);
    messageLabel->hide();
    mainLayout->addWidget(messageLabel);

    mainLayout->addStretch();


    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(usernameInput, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(passwordInput, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::applyStyles() {

    setStyleSheet(
        "QWidget {"
        "    background-color: #1a1a2e;"
        "}"
        "QLabel#titleLabel {"
        "    color: #0f3460;"
        "    background-color: transparent;"
        "}"
        "QLineEdit {"
        "    background-color: #16213e;"
        "    color: #eaeaea;"
        "    border: 2px solid #0f3460;"
        "    border-radius: 8px;"
        "    padding: 8px 15px;"
        "    font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #533483;"
        "    background-color: #1a1a2e;"
        "}"
        "QPushButton {"
        "    background-color: #533483;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #6a4c93;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d2a5f;"
        "}"
        "QLabel {"
        "    color: #eaeaea;"
        "    font-size: 12px;"
        "}"
    );


    titleLabel->setStyleSheet(
        "color: #533483;"
        "background-color: transparent;"
    );
}

bool LoginWindow::validateUsername(const QString& username) {
    QString trimmed = username.trimmed();
    
    if (trimmed.isEmpty()) {
        messageLabel->setText("Introduceți un nume de utilizator!");
        messageLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");
        messageLabel->show();
        return false;
    }
    
    if (trimmed.length() < 3) {
        messageLabel->setText("Numele de utilizator trebuie să aibă minim 3 caractere!");
        messageLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");
        messageLabel->show();
        return false;
    }
    
    return true;
}

void LoginWindow::onLoginClicked() {
    messageLabel->hide();
    
    QString username = usernameInput->text();
    QString password = passwordInput->text();
    
    if (!validateUsername(username)) return;
    if (password.isEmpty()) {
         messageLabel->setText("Introduceți parola!");
         messageLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");
         messageLabel->show();
         return;
    }
    
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    messageLabel->setText("Conectare...");
    messageLabel->setStyleSheet("color: #4ecdc4; font-size: 12px;");
    messageLabel->show();
    

    QTimer::singleShot(100, [this, username, password]() {
        if (gameClient && gameClient->Login(username.toStdString(), password.toStdString())) {
            messageLabel->setText("Login reușit! Redirecționare...");
            messageLabel->setStyleSheet("color: #51cf66; font-size: 12px;");
            messageLabel->show();
            
            QTimer::singleShot(500, [this, username]() {
                emit loginSuccessful(username);
            });
        } else {
            messageLabel->setText("Eroare la conectare! Verificați credențialele.");
            messageLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");
            messageLabel->show();
            loginButton->setEnabled(true);
            registerButton->setEnabled(true);
        }
    });
}

void LoginWindow::onRegisterClicked() {
    messageLabel->hide();
    
    QString username = usernameInput->text();
    QString password = passwordInput->text();
    
    if (!validateUsername(username)) return;
    if (password.length() < 3) {
         messageLabel->setText("Parola trebuie să aibă minim 3 caractere!");
         messageLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");
         messageLabel->show();
         return;
    }
    

    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    messageLabel->setText("Înregistrare...");
    messageLabel->setStyleSheet("color: #4ecdc4; font-size: 12px;");
    messageLabel->show();
    
    QTimer::singleShot(500, [this, username, password]() {
        if (gameClient && gameClient->Register(username.toStdString(), password.toStdString())) {
            messageLabel->setText("Înregistrare reușită! Acum vă puteți autentifica.");
            messageLabel->setStyleSheet("color: #51cf66; font-size: 12px;");
            messageLabel->show();
            
            loginButton->setEnabled(true);
            registerButton->setEnabled(true);
        } else {
            messageLabel->setText("Eroare la înregistrare! (Posibil user existent)");
            messageLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");
            messageLabel->show();
            loginButton->setEnabled(true);
            registerButton->setEnabled(true);
        }
    });
}
