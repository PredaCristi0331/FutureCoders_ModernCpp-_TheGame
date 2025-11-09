#include "LoginWindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QFont>
#include <QPalette>

// Inițializare date mock
QSet<QString> LoginWindow::mockUsers;

LoginWindow::LoginWindow(QWidget* parent) : QWidget(parent) {
    initializeMockUsers();
    setupUI();
    applyStyles();
}

LoginWindow::~LoginWindow() {}

void LoginWindow::initializeMockUsers() {
    if (mockUsers.isEmpty()) {
        mockUsers.insert("test1");
        mockUsers.insert("test2");
        mockUsers.insert("admin");
        mockUsers.insert("user123");
    }
}

void LoginWindow::setupUI() {
    // Nu setăm windowTitle și resize când avem parent (sunt setate de QMainWindow)
    setMinimumSize(400, 300);

    // Layout principal
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Titlu
    titleLabel = new QLabel("THE GAME", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Arial", 28, QFont::Bold);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Spacer
    mainLayout->addStretch();

    // Input username
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Introdu numele de utilizator");
    usernameInput->setMinimumHeight(40);
    mainLayout->addWidget(usernameInput);

    // Butoane
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

    // Mesaj de eroare/succes
    messageLabel = new QLabel(this);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setWordWrap(true);
    messageLabel->setMinimumHeight(30);
    messageLabel->hide();
    mainLayout->addWidget(messageLabel);

    mainLayout->addStretch();

    // Conectări
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(usernameInput, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::applyStyles() {
    // Stilizare personalizată - design modern
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

    // Stilizare titlu separat
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
    
    if (trimmed.length() > 20) {
        messageLabel->setText("Numele de utilizator trebuie să aibă maxim 20 de caractere!");
        messageLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");
        messageLabel->show();
        return false;
    }
    
    return true;
}

bool LoginWindow::mockLogin(const QString& username) {
    QString trimmed = username.trimmed();
    
    if (mockUsers.contains(trimmed)) {
        return true;
    }
    
    return false;
}

bool LoginWindow::mockRegister(const QString& username) {
    QString trimmed = username.trimmed();
    
    if (mockUsers.contains(trimmed)) {
        return false; // User deja există
    }
    
    // Adaugă user nou în lista mock
    mockUsers.insert(trimmed);
    return true;
}

void LoginWindow::onLoginClicked() {
    messageLabel->hide();
    
    QString username = usernameInput->text();
    
    if (!validateUsername(username)) {
        return;
    }
    
    // Simulare delay pentru a simula comunicarea cu serverul
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    messageLabel->setText("Conectare...");
    messageLabel->setStyleSheet("color: #4ecdc4; font-size: 12px;");
    messageLabel->show();
    
    QTimer::singleShot(500, [this, username]() {
        if (mockLogin(username)) {
            messageLabel->setText("Login reușit! Redirecționare...");
            messageLabel->setStyleSheet("color: #51cf66; font-size: 12px;");
            messageLabel->show();
            
            // Emite semnal pentru succes
            QTimer::singleShot(1000, [this, username]() {
                emit loginSuccessful(username);
            });
        } else {
            messageLabel->setText("Username-ul nu există! Încercați să vă înregistrați.");
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
    
    if (!validateUsername(username)) {
        return;
    }
    
    // Simulare delay pentru a simula comunicarea cu serverul
    loginButton->setEnabled(false);
    registerButton->setEnabled(false);
    messageLabel->setText("Înregistrare...");
    messageLabel->setStyleSheet("color: #4ecdc4; font-size: 12px;");
    messageLabel->show();
    
    QTimer::singleShot(500, [this, username]() {
        if (mockRegister(username)) {
            messageLabel->setText("Înregistrare reușită! Redirecționare...");
            messageLabel->setStyleSheet("color: #51cf66; font-size: 12px;");
            messageLabel->show();
            
            // Emite semnal pentru succes
            QTimer::singleShot(1000, [this, username]() {
                emit loginSuccessful(username);
            });
        } else {
            messageLabel->setText("Username-ul există deja! Încercați să vă logați.");
            messageLabel->setStyleSheet("color: #ff6b6b; font-size: 12px;");
            messageLabel->show();
            loginButton->setEnabled(true);
            registerButton->setEnabled(true);
        }
    });
}

