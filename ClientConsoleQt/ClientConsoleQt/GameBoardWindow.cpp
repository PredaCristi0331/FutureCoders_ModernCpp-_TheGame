#include "GameBoardWindow.h"
#include "GameClient.h"
#include "CardWidget.h"
#include "PileWidget.h"
#include "OpponentWidget.h"
#include <QSplitter>
#include <QMessageBox>

GameBoardWindow::GameBoardWindow(GameClient* client, QWidget* parent)
    : QWidget(parent)
    , gameClient(client)
{
    setupUI();
    setupNotificationUI(); // Inline Init
    applyStyles();
    
    // Test Notification
    QTimer::singleShot(1000, [this](){ showNotification("Bun venit la masă!"); });
}

GameBoardWindow::~GameBoardWindow() = default;

void GameBoardWindow::setUsername(const QString& username) {
    currentUsername = username;
    // Load initial state or mock data here
    updateHandUI();
}

void GameBoardWindow::setupUI() {
    auto* mainLayout = new QHBoxLayout(this);
    
    // Left side: Game Area (80%)
    auto* gameLayout = new QVBoxLayout();
    
    // 1. Opponents Area (Top)
    opponentsArea = new QWidget(this);
    opponentsArea->setFixedHeight(120);
    auto* oppLayout = new QHBoxLayout(opponentsArea);
    oppLayout->setAlignment(Qt::AlignCenter);
    oppLayout->setSpacing(30);

    // Mock Opponents for now (Will be dynamic later)
    QStringList mockNames = {"Alex", "Maria", "John"};
    for (const auto& name : mockNames) {
        auto* opp = new OpponentWidget(name, this);
        opponents.push_back(opp);
        oppLayout->addWidget(opp);
    }
    
    // 2. Piles Area (Center)
    pilesArea = new QWidget(this);
    auto* pileLayout = new QHBoxLayout(pilesArea);
    pileLayout->setAlignment(Qt::AlignCenter);
    pileLayout->setSpacing(20);

    // Create 4 piles: 2 Ascending, 2 Descending
    // Order: Asc, Asc, Deck, Desc, Desc
    
    // Ascending 1
    auto* p1 = new PileWidget(PileType::Ascending_1_to_99, this);
    piles.push_back(p1);
    pileLayout->addWidget(p1);
    
    // Ascending 2
    auto* p2 = new PileWidget(PileType::Ascending_1_to_99, this);
    piles.push_back(p2);
    pileLayout->addWidget(p2);

    // Deck Center
    auto* deckContainer = new QVBoxLayout();
    deckLabel = new QLabel("🂠", this); // Card back unicode
    deckLabel->setStyleSheet("font-size: 64px; color: #6c5ce7;");
    deckLabel->setAlignment(Qt::AlignCenter);
    
    cardsRemainingLabel = new QLabel("98", this);
    cardsRemainingLabel->setAlignment(Qt::AlignCenter);
    cardsRemainingLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    
    deckContainer->addWidget(deckLabel);
    deckContainer->addWidget(cardsRemainingLabel);
    pileLayout->addLayout(deckContainer);

    // Descending 1
    auto* p3 = new PileWidget(PileType::Descending_100_to_2, this);
    piles.push_back(p3);
    pileLayout->addWidget(p3);
    
    // Descending 2
    auto* p4 = new PileWidget(PileType::Descending_100_to_2, this);
    piles.push_back(p4);
    pileLayout->addWidget(p4);

    // Connect signals
    for (auto* p : piles) {
        connect(p, &PileWidget::clicked, this, &GameBoardWindow::onPileClicked);
    }
    
    // 3. Hand Area (Bottom)
    handArea = new QWidget(this);
    handArea->setFixedHeight(150);
    auto* handLayout = new QHBoxLayout(handArea);
    handLayout->setAlignment(Qt::AlignCenter);
    
    gameLayout->addWidget(opponentsArea);
    gameLayout->addWidget(pilesArea);
    gameLayout->addWidget(handArea);
    
    // Right side: Chat/Log (20%)
    auto* rightPanel = new QWidget(this);
    rightPanel->setFixedWidth(250);
    auto* rightLayout = new QVBoxLayout(rightPanel);
    
    setupChatUI(rightLayout); // Commit 3 logic

    mainLayout->addLayout(gameLayout);
    mainLayout->addWidget(rightPanel);
}

void GameBoardWindow::setupChatUI(QVBoxLayout* layout) {
    auto* chatTitle = new QLabel("Chat & Log", this);
    chatTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(chatTitle);

    chatHistory = new QListWidget(this);
    layout->addWidget(chatHistory);

    auto* inputLayout = new QHBoxLayout();
    chatInput = new QLineEdit(this);
    chatInput->setPlaceholderText("Scrie un mesaj...");
    
    sendChatButton = new QPushButton("Trimite", this);
    
    inputLayout->addWidget(chatInput);
    inputLayout->addWidget(sendChatButton);
    
    layout->addLayout(inputLayout);

    connect(sendChatButton, &QPushButton::clicked, this, &GameBoardWindow::onSendChatClicked);
    connect(chatInput, &QLineEdit::returnPressed, this, &GameBoardWindow::onSendChatClicked);
}

void GameBoardWindow::updateHandUI() {
    // Mock hand for visual verification
    if (handArea->layout()) {
        QLayoutItem* item;
        while ((item = handArea->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    handCards.clear();
    // Add 5 mock cards
    for (int i = 1; i <= 5; ++i) {
        int val = i * 10 + (rand() % 9);
        auto* card = new CardWidget(val, this);
        connect(card, &CardWidget::clicked, this, &GameBoardWindow::onCardClicked);
        handArea->layout()->addWidget(card);
        handCards.push_back(card);
    }
}

void GameBoardWindow::onCardClicked(int value) {
    // Deselect all
    for (auto* card : handCards) {
        card->setSelected(false);
        if (card->getValue() == value) {
            card->setSelected(true);
        }
    }
    // Logic to play card to follow...
}

void GameBoardWindow::onPileClicked(PileType type) {
    if (selectedHandIndex != -1) {
        // Here we would implement the logic to play the card
        // GameClient::PlayCard(...)
        QMessageBox::information(this, "Acțiune", "Ai încercat să joci o carte!");
    }
}
    QString msg = chatInput->text().trimmed();
    if (msg.isEmpty()) return;

    // Add to local UI immediately (optimistic UI)
    chatHistory->addItem(QString("[%1]: %2").arg(currentUsername, msg));
    chatInput->clear();
    
    if (gameClient) {
        gameClient->SendChat(msg.toStdString());
    }
}

void GameBoardWindow::applyStyles() {
    setStyleSheet(
        "QWidget {"
        "    background-color: #1a1a2e;"
        "    color: #eaeaea;"
        "}"
        "QListWidget {"
        "    background-color: #16213e;"
        "    border: 1px solid #0f3460;"
        "    border-radius: 4px;"
        "}"
        "QLineEdit {"
        "    background-color: #16213e;"
        "    border: 1px solid #0f3460;"
        "    padding: 5px;"
        "    border-radius: 4px;"
        "}"
        "QPushButton {"
        "    background-color: #533483;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "}"
    );
     if (opponentsArea) opponentsArea->setStyleSheet("background-color: #16213e; border-bottom: 2px solid #0f3460;");
     if (pilesArea) pilesArea->setStyleSheet("background-color: #1a1a2e;");
     if (handArea) handArea->setStyleSheet("background-color: #16213e; border-top: 2px solid #0f3460;");
     
     if (notificationLabel) {
         notificationLabel->setStyleSheet(
            "background-color: rgba(0, 0, 0, 0.7);"
            "color: #fab1a0;"
            "font-size: 20px;"
            "font-weight: bold;"
            "padding: 10px 20px;"
            "border-radius: 10px;"
            "border: 1px solid #fab1a0;"
         );
     }
}

// --- Inline Commit 3: Notification Logic ---
void GameBoardWindow::setupNotificationUI() {
    notificationLabel = new QLabel(this);
    notificationLabel->setAlignment(Qt::AlignCenter);
    notificationLabel->setAttribute(Qt::WA_TransparentForMouseEvents); // Click through
    notificationLabel->hide();
    
    // Center logic handled in resizeEvent usually, or manual geometry
    notificationLabel->setFixedWidth(400);
    notificationLabel->setFixedHeight(60);
    // Position mock
    notificationLabel->move(width()/2 - 200, 150); 
}

void GameBoardWindow::showNotification(const QString& message) {
    if(!notificationLabel) return;
    
    notificationLabel->setText(message);
    notificationLabel->adjustSize();
    
    // Re-center
    int w = notificationLabel->width() + 40;
    int h = notificationLabel->height() + 20;
    notificationLabel->setFixedSize(w, h);
    notificationLabel->move((this->width() - w)/2, 80); // Top center
    
    notificationLabel->show();
    
    // Fade effect
    auto* effect = new QGraphicsOpacityEffect(notificationLabel);
    notificationLabel->setGraphicsEffect(effect);
    
    auto* anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(2500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::InExpo);
    
    connect(anim, &QPropertyAnimation::finished, [this]() {
        notificationLabel->hide();
    });
    
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
