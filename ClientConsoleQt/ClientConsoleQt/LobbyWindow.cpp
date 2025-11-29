#include "LobbyWindow.h"
#include <QFont>

LobbyWindow::LobbyWindow(QWidget* parent) 
    : QWidget(parent)
    , selectedDifficulty("Ușor")
    , matchmakingTimer(nullptr)
{
    setupUI();
    applyStyles();
    connectSignals();
}

LobbyWindow::~LobbyWindow() {
    if (matchmakingTimer) {
        matchmakingTimer->stop();
        delete matchmakingTimer;
    }
}

void LobbyWindow::setUsername(const QString& username) {
    currentUsername = username;
    if (welcomeLabel) {
        welcomeLabel->setText(QString("Bine ai venit, %1!").arg(username));
    }
}

void LobbyWindow::setupUI() {
    // Layout principal
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Titlu de bun venit
    welcomeLabel = new QLabel("Bine ai venit!", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Arial", 24, QFont::Bold);
    welcomeLabel->setFont(titleFont);
    mainLayout->addWidget(welcomeLabel);

    mainLayout->addStretch();

    // Selector dificultate
    QVBoxLayout* difficultyLayout = new QVBoxLayout();
    difficultyLayout->setSpacing(10);

    difficultyLabel = new QLabel("Selectează nivelul de dificultate:", this);
    difficultyLabel->setAlignment(Qt::AlignCenter);
    difficultyLayout->addWidget(difficultyLabel);

    difficultyComboBox = new QComboBox(this);
    difficultyComboBox->addItem("Ușor");
    difficultyComboBox->addItem("Mediu");
    difficultyComboBox->addItem("Greu");
    difficultyComboBox->setMinimumHeight(45);
    difficultyComboBox->setCurrentIndex(0);
    difficultyLayout->addWidget(difficultyComboBox);

    mainLayout->addLayout(difficultyLayout);

    // Butoane
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(15);

    playButton = new QPushButton("Caută Joc", this);
    playButton->setMinimumHeight(50);
    playButton->setMinimumWidth(200);
    buttonLayout->addWidget(playButton);

    profileButton = new QPushButton("Profil", this);
    profileButton->setMinimumHeight(45);
    profileButton->setMinimumWidth(200);
    buttonLayout->addWidget(profileButton);

    logoutButton = new QPushButton("Logout", this);
    logoutButton->setMinimumHeight(45);
    logoutButton->setMinimumWidth(200);
    buttonLayout->addWidget(logoutButton);

    mainLayout->addLayout(buttonLayout);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setMinimumHeight(40);
    statusLabel->hide();
    mainLayout->addWidget(statusLabel);

    mainLayout->addStretch();
}

void LobbyWindow::applyStyles() {
    setStyleSheet(
        "QWidget {"
        "    background-color: #1a1a2e;"
        "}"
        "QLabel {"
        "    color: #eaeaea;"
        "    font-size: 14px;"
        "}"
        "QComboBox {"
        "    background-color: #16213e;"
        "    color: #eaeaea;"
        "    border: 2px solid #0f3460;"
        "    border-radius: 8px;"
        "    padding: 8px 15px;"
        "    font-size: 14px;"
        "}"
        "QComboBox:hover {"
        "    border: 2px solid #533483;"
        "}"
        "QComboBox:focus {"
        "    border: 2px solid #533483;"
        "    background-color: #1a1a2e;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    width: 30px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 5px solid #eaeaea;"
        "    width: 0;"
        "    height: 0;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: #16213e;"
        "    color: #eaeaea;"
        "    selection-background-color: #533483;"
        "    border: 2px solid #0f3460;"
        "    border-radius: 8px;"
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
        "QPushButton:disabled {"
        "    background-color: #2a2a3e;"
        "    color: #888888;"
        "}"
    );

    // Stilizare titlu
    welcomeLabel->setStyleSheet(
        "color: #533483;"
        "background-color: transparent;"
        "font-size: 24px;"
    );

    // Stilizare label dificultate
    difficultyLabel->setStyleSheet(
        "color: #eaeaea;"
        "font-size: 16px;"
        "font-weight: bold;"
    );
}

void LobbyWindow::connectSignals() {
    connect(playButton, &QPushButton::clicked, this, &LobbyWindow::onPlayClicked);
    connect(profileButton, &QPushButton::clicked, this, &LobbyWindow::onProfileClicked);
    connect(logoutButton, &QPushButton::clicked, this, &LobbyWindow::onLogoutClicked);
    connect(difficultyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LobbyWindow::onDifficultyChanged);
}

void LobbyWindow::onDifficultyChanged(int index) {
    QString difficulties[] = {"Ușor", "Mediu", "Greu"};
    if (index >= 0 && index < 3) {
        selectedDifficulty = difficulties[index];
    }
}

void LobbyWindow::onPlayClicked() {
    if (!currentUsername.isEmpty()) {
        // Dezactivează butoanele
        playButton->setEnabled(false);
        profileButton->setEnabled(false);
        logoutButton->setEnabled(false);
        difficultyComboBox->setEnabled(false);

        // Afișează status de matchmaking
        updateStatus("Căutare joc...", "#4ecdc4");
        
        // Simulează matchmaking (2-3 secunde)
        if (!matchmakingTimer) {
            matchmakingTimer = new QTimer(this);
            matchmakingTimer->setSingleShot(true);
            connect(matchmakingTimer, &QTimer::timeout, this, &LobbyWindow::onMatchmakingTimeout);
        }
        
        matchmakingTimer->start(2000); // 2 secunde pentru simulare
    }
}

void LobbyWindow::onMatchmakingTimeout() {
    // Joc găsit - emite semnal pentru a începe jocul
    updateStatus("Joc găsit! Pornire joc...", "#51cf66");
    
    // Așteaptă puțin apoi emite semnalul
    QTimer::singleShot(1000, [this]() {
        emit startGame(currentUsername, selectedDifficulty);
    });
}

void LobbyWindow::onProfileClicked() {
    emit showProfile(currentUsername);
}

void LobbyWindow::onLogoutClicked() {
    emit logout();
}

void LobbyWindow::updateStatus(const QString& message, const QString& color) {
    statusLabel->setText(message);
    statusLabel->setStyleSheet(QString("color: %1; font-size: 14px; font-weight: bold;").arg(color));
    statusLabel->show();
}

