#include "LobbyWindow.h"
#include "RulesWindow.h"
#include "GameClient.h"
#include <QFont>
#include <QInputDialog>

LobbyWindow::LobbyWindow(GameClient* client, QWidget* parent) 
    : QWidget(parent)
    , gameClient(client)
    , selectedDifficulty("Ușor")
    , matchmakingTimer(new QTimer(this))
{
    setupUI();
    setupSettingsUI(); // Init Settings
    setupWaitingUI();  // Init Waiting
    
    // Default hidden
    if(settingsOverlay) settingsOverlay->hide();
    if(waitingOverlay) waitingOverlay->hide();

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

    // Selector Nr Jucatori
    QVBoxLayout* playersLayout = new QVBoxLayout();
    playersLayout->setSpacing(10);
    
    numPlayersLabel = new QLabel("Număr de jucători:", this);
    numPlayersLabel->setAlignment(Qt::AlignCenter);
    numPlayersLabel->setStyleSheet("color: #eaeaea; font-size: 16px; font-weight: bold;");
    playersLayout->addWidget(numPlayersLabel);
    
    numPlayersComboBox = new QComboBox(this);
    numPlayersComboBox->addItem("2 Jucători");
    numPlayersComboBox->addItem("3 Jucători");
    numPlayersComboBox->addItem("4 Jucători"); // If 4 is maxSupported, or add 5 if rules allow
    numPlayersComboBox->addItem("5 Jucători");
    numPlayersComboBox->setMinimumHeight(45);
    numPlayersComboBox->setCurrentIndex(0); // Default 2
    playersLayout->addWidget(numPlayersComboBox);
    
    mainLayout->addLayout(playersLayout);

    // Butoane
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(15);

    // 1. Join Game
    quickMatchButton = new QPushButton("Join Game", this);
    quickMatchButton->setMinimumHeight(45);
    quickMatchButton->setStyleSheet("background-color: #27ae60; font-weight: bold;"); // Green
    buttonLayout->addWidget(quickMatchButton);

    // 2. Create Game
    createGameButton = new QPushButton("Crează Joc", this);
    createGameButton->setMinimumHeight(45);
    createGameButton->setStyleSheet("background-color: #2980b9; font-weight: bold;"); // Blue
    buttonLayout->addWidget(createGameButton);

    profileButton = new QPushButton("Profil", this);
    profileButton->setMinimumHeight(45);
    profileButton->setMinimumWidth(200);
    buttonLayout->addWidget(profileButton);

    rulesButton = new QPushButton("Regulament", this);
    rulesButton->setMinimumHeight(45);
    rulesButton->setMinimumWidth(200);
    // Style specific
    rulesButton->setStyleSheet("background-color: #0f3460;"); // Distinct color
    buttonLayout->addWidget(rulesButton);

    buttonLayout->addWidget(rulesButton);
    
    // Add small Settings button near Rules
    settingsButton = new QPushButton("⚙ Setări", this);
    settingsButton->setMinimumHeight(45);
    settingsButton->setStyleSheet("background-color: #2d3436; color: #dfe6e9;");
    buttonLayout->addWidget(settingsButton);

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
        "    padding: 10px;"
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
    connect(quickMatchButton, &QPushButton::clicked, this, &LobbyWindow::onJoinGameClicked);
    connect(createGameButton, &QPushButton::clicked, this, &LobbyWindow::onCreateGameClicked);
    connect(profileButton, &QPushButton::clicked, this, &LobbyWindow::onProfileClicked);
    connect(rulesButton, &QPushButton::clicked, this, &LobbyWindow::onRulesClicked);
    connect(settingsButton, &QPushButton::clicked, this, &LobbyWindow::toggleSettings);
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

void LobbyWindow::onJoinGameClicked() {
    if (currentUsername.isEmpty()) return;
    
    bool ok;
    int gameId = QInputDialog::getInt(this, "Join Game", 
                                      "Introduceți ID-ul jocului:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    updateStatus(QString("Conectare la jocul %1...").arg(gameId), "#f39c12");
    
    QTimer::singleShot(100, [this, gameId](){
        if(gameClient->JoinGame(gameId)) {
             updateStatus("Conectat! Așteptare jucători...", "#2ecc71");
             showWaitingScreen();
        } else {
            updateStatus("Eroare: Jocul nu există sau este plin.", "#c0392b");
        }
    });
}

void LobbyWindow::onCreateGameClicked() {
    if (currentUsername.isEmpty()) return;
    
    updateStatus("Se crează joc...", "#f39c12");
    
    QTimer::singleShot(100, [this](){
        // Get selected player count: index 0 -> 2 players, 1 -> 3 players...
        int players = numPlayersComboBox->currentIndex() + 2; 
        
        if(gameClient->CreateGame(players)) {
            int newGameId = gameClient->GetGameId();
            updateStatus(QString("Joc creat (ID: %1)! Așteptare jucători...").arg(newGameId), "#2ecc71");
            showWaitingScreen();
        } else {
            updateStatus("Eroare la creare.", "#c0392b");
        }
    });
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

void LobbyWindow::onRulesClicked() {
    RulesWindow rules(this);
    rules.exec();
}

void LobbyWindow::onLogoutClicked() {
    emit logout();
}

void LobbyWindow::updateStatus(const QString& message, const QString& color) {
    statusLabel->setText(message);
    statusLabel->setStyleSheet(QString("color: %1; font-size: 14px; font-weight: bold;").arg(color));
    statusLabel->show();
}

// --- Inline Commit 1: Settings Logic ---
void LobbyWindow::setupSettingsUI() {
    settingsOverlay = new QWidget(this);
    settingsOverlay->setGeometry(rect()); // Cover entire window
    settingsOverlay->setStyleSheet("background-color: rgba(0,0,0,0.85);");
    
    auto* layout = new QVBoxLayout(settingsOverlay);
    layout->setAlignment(Qt::AlignCenter);
    
    auto* container = new QFrame(settingsOverlay);
    container->setFixedSize(300, 400);
    container->setStyleSheet("background-color: #1a1a2e; border: 2px solid #533483; border-radius: 10px;");
    
    auto* innerLayout = new QVBoxLayout(container);
    
    auto* title = new QLabel("SETĂRI", container);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #533483; border: none;");
    
    soundToggle = new QCheckBox("Efecte Sonore", container);
    soundToggle->setChecked(true);
    
    musicToggle = new QCheckBox("Muzică Fundal", container);
    musicToggle->setChecked(true);
    
    auto* themeLabel = new QLabel("Temă:", container);
    themeLabel->setStyleSheet("border:none;");
    themeCombo = new QComboBox(container);
    themeCombo->addItem("Dark Space");
    themeCombo->addItem("Light Minimal");
    connect(themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LobbyWindow::onThemeChanged);
    
    closeSettingsBtn = new QPushButton("Salvează & Închide", container);
    connect(closeSettingsBtn, &QPushButton::clicked, this, &LobbyWindow::toggleSettings);
    
    innerLayout->addWidget(title);
    innerLayout->addSpacing(20);
    innerLayout->addWidget(soundToggle);
    innerLayout->addWidget(musicToggle);
    innerLayout->addSpacing(10);
    innerLayout->addWidget(themeLabel);
    innerLayout->addWidget(themeCombo);
    innerLayout->addStretch();
    innerLayout->addWidget(closeSettingsBtn);
    
    layout->addWidget(container);
}

void LobbyWindow::toggleSettings() {
    if (settingsOverlay->isVisible()) {
        settingsOverlay->hide();
    } else {
        settingsOverlay->setGeometry(rect()); // Ensure it covers updated size
        settingsOverlay->show();
        settingsOverlay->raise();
    }
}

void LobbyWindow::onThemeChanged(int index) {
    // Mock theme implementation
    if (index == 1) {
       // Light Mode Mock
    }
}

// --- Inline Commit 2: Waiting Logic ---
void LobbyWindow::setupWaitingUI() {
    waitingOverlay = new QWidget(this);
    waitingOverlay->setGeometry(rect());
    waitingOverlay->setStyleSheet("background-color: #1a1a2e;"); // Solid cover
    
    auto* layout = new QVBoxLayout(waitingOverlay);
    layout->setAlignment(Qt::AlignCenter);
    
    waitingStatusLabel = new QLabel("Așteptare jucători...", waitingOverlay);
    waitingStatusLabel->setAlignment(Qt::AlignCenter);
    waitingStatusLabel->setStyleSheet("font-size: 24px; color: #4ecdc4; margin-bottom: 20px;");
    
    gameIdDisplayLabel = new QLabel("Game ID: -", waitingOverlay);
    gameIdDisplayLabel->setAlignment(Qt::AlignCenter);
    gameIdDisplayLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #f1c40f; margin-bottom: 10px;");
    
    playerCountLabel = new QLabel("Jucători: -/-", waitingOverlay);
    playerCountLabel->setAlignment(Qt::AlignCenter);
    playerCountLabel->setStyleSheet("font-size: 18px; color: #ecf0f1;");
    
    cancelWaitBtn = new QPushButton("Anulează", waitingOverlay);
    cancelWaitBtn->setFixedWidth(150);
    cancelWaitBtn->setStyleSheet("background-color: #e74c3c; margin-top: 30px;");
    connect(cancelWaitBtn, &QPushButton::clicked, this, &LobbyWindow::hideWaitingScreen);
    
    layout->addWidget(waitingStatusLabel);
    layout->addWidget(gameIdDisplayLabel);
    layout->addWidget(playerCountLabel);
    layout->addWidget(cancelWaitBtn);
}

void LobbyWindow::showWaitingScreen() {
    waitingOverlay->setGeometry(rect());
    waitingOverlay->show();
    waitingOverlay->raise();
    
    // Set static info
    gameIdDisplayLabel->setText(QString("Game ID: %1").arg(gameClient->GetGameId()));
    
    disconnect(matchmakingTimer, nullptr, nullptr, nullptr);
    connect(matchmakingTimer, &QTimer::timeout, [this]() {
        // Poll Server
        if (gameClient->PollGameState()) {
            GameState state = gameClient->GetGameState();
            
            playerCountLabel->setText(QString("Jucători: %1 / %2")
                                      .arg(state.currentPlayers)
                                      .arg(state.maxPlayers));

            if (state.status == "playing") {
                 onMatchmakingTimeout(); // Reuse start logic
                 return;
            }
        }
    });
    matchmakingTimer->start(1000); // Poll every second
    
    // Initial poll
    gameClient->PollGameState();
}

void LobbyWindow::hideWaitingScreen() {
    waitingOverlay->hide();
    matchmakingTimer->stop();
    quickMatchButton->setEnabled(true);
    createGameButton->setEnabled(true);
    profileButton->setEnabled(true);
    settingsButton->setEnabled(true); // Re-enable
}

