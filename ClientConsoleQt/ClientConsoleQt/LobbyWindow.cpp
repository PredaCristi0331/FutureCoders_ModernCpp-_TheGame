#include "LobbyWindow.h"
#include "RulesWindow.h"
#include <QFont>

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
    connect(playButton, &QPushButton::clicked, this, &LobbyWindow::onPlayClicked);
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

void LobbyWindow::onPlayClicked() {
    if (!currentUsername.isEmpty()) {
        // UI State Update
        playButton->setEnabled(false);
        profileButton->setEnabled(false);
        logoutButton->setEnabled(false);
        difficultyComboBox->setEnabled(false);
        
        // Visual Feedback
        playButton->setText("Se caută adversar...");
        // Orange color for "searching" state
        playButton->setStyleSheet("background-color: #d35400; color: white; border-radius: 8px; font-weight: bold; font-size: 14px;");

        // Afișează status de matchmaking
        // updateStatus("Căutare joc...", "#4ecdc4"); // Old
        
        // New Waiting UI
        showWaitingScreen();
        
        // Logic moved to showWaitingScreen()
        // timer logic...
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
    
    waitingStatusLabel = new QLabel("Se caută jucători...", waitingOverlay);
    waitingStatusLabel->setAlignment(Qt::AlignCenter);
    waitingStatusLabel->setStyleSheet("font-size: 24px; color: #4ecdc4;");
    
    waitingProgress = new QProgressBar(waitingOverlay);
    waitingProgress->setRange(0, 30);
    waitingProgress->setValue(0);
    waitingProgress->setFixedWidth(300);
    waitingProgress->setStyleSheet(
        "QProgressBar { border: 2px solid #533483; border-radius: 5px; text-align: center; }"
        "QProgressBar::chunk { background-color: #533483; }"
    );
    
    cancelWaitBtn = new QPushButton("Anulează", waitingOverlay);
    cancelWaitBtn->setFixedWidth(150);
    cancelWaitBtn->setStyleSheet("background-color: #e74c3c;");
    connect(cancelWaitBtn, &QPushButton::clicked, this, &LobbyWindow::hideWaitingScreen);
    
    layout->addWidget(waitingStatusLabel);
    layout->addSpacing(20);
    layout->addWidget(waitingProgress);
    layout->addSpacing(30);
    layout->addWidget(cancelWaitBtn);
}

void LobbyWindow::showWaitingScreen() {
    waitingOverlay->setGeometry(rect());
    waitingOverlay->show();
    waitingOverlay->raise();
    
    // Start progress
    waitingProgress->setValue(0);
    // Reuse existing timer but connect to progress update
    disconnect(matchmakingTimer, nullptr, nullptr, nullptr);
    connect(matchmakingTimer, &QTimer::timeout, [this]() {
        int val = waitingProgress->value();
        if (val >= 30) {
            onMatchmakingTimeout();
        } else {
            waitingProgress->setValue(val + 1);
            if (val % 5 == 0) {
                waitingStatusLabel->setText(QString("Jucători găsiți: %1/4").arg(1 + val/10));
            }
        }
    });
    matchmakingTimer->start(100); // Fast for demo (should be 1000 for seconds)
}

void LobbyWindow::hideWaitingScreen() {
    waitingOverlay->hide();
    matchmakingTimer->stop();
    playButton->setEnabled(true);
    profileButton->setEnabled(true);
    settingsButton->setEnabled(true); // Re-enable
}

