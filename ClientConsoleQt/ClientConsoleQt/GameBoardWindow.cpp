#include "GameBoardWindow.h"
#include "GameClient.h"
#include "CardWidget.h"
#include "PileWidget.h"
#include "OpponentWidget.h"
#include "GameOverDialog.h"
#include <QSplitter>
#include <QMessageBox>
#include <QTimer>

GameBoardWindow::GameBoardWindow(GameClient* client, QWidget* parent)
    : QWidget(parent)
    , gameClient(client)
{
    setupUI();
    setupNotificationUI();
    applyStyles();
    
    connect(gameClient, &GameClient::gameStateUpdated, this, &GameBoardWindow::updateGameState);
    connect(gameClient, &GameClient::chatUpdated, this, &GameBoardWindow::onChatUpdated);

    QTimer::singleShot(100, [this](){ 
        if(gameClient->IsInGame()) {
            gameClient->PollGameState(); 
            gameClient->PollChat(); 
        }
    });
}

GameBoardWindow::~GameBoardWindow() = default;

void GameBoardWindow::setUsername(const QString& username) {
    currentUsername = username;
}

void GameBoardWindow::setupUI() {
    auto* mainLayout = new QHBoxLayout(this);
    
    auto* gameLayout = new QVBoxLayout();
    
    opponentsArea = new QWidget(this);
    opponentsArea->setFixedHeight(120);
    auto* oppLayout = new QHBoxLayout(opponentsArea);
    oppLayout->setAlignment(Qt::AlignCenter);
    oppLayout->setSpacing(30);

    pilesArea = new QWidget(this);
    auto* pileLayout = new QHBoxLayout(pilesArea);
    pileLayout->setAlignment(Qt::AlignCenter);
    pileLayout->setSpacing(20);

    auto* p1 = new PileWidget(PileType::Ascending_1_to_99, this);
    piles.push_back(p1);
    pileLayout->addWidget(p1);
    
    auto* p2 = new PileWidget(PileType::Ascending_1_to_99, this);
    piles.push_back(p2);
    pileLayout->addWidget(p2);

    auto* deckContainer = new QVBoxLayout();
    deckLabel = new QLabel("🂠", this);
    deckLabel->setStyleSheet("font-size: 64px; color: #6c5ce7;");
    deckLabel->setAlignment(Qt::AlignCenter);
    
    cardsRemainingLabel = new QLabel("98", this);
    cardsRemainingLabel->setAlignment(Qt::AlignCenter);
    cardsRemainingLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    
    deckContainer->addWidget(deckLabel);
    deckContainer->addWidget(cardsRemainingLabel);
    pileLayout->addLayout(deckContainer);

    auto* p3 = new PileWidget(PileType::Descending_100_to_2, this);
    piles.push_back(p3);
    pileLayout->addWidget(p3);
    
    auto* p4 = new PileWidget(PileType::Descending_100_to_2, this);
    piles.push_back(p4);
    pileLayout->addWidget(p4);

    for (auto* p : piles) {
        connect(p, &PileWidget::clicked, this, &GameBoardWindow::onPileClicked);
    }
    
    handArea = new QWidget(this);
    handArea->setFixedHeight(150);
    auto* handLayout = new QHBoxLayout(handArea);
    handLayout->setAlignment(Qt::AlignCenter);
    
    turnLabel = new QLabel("Waiting...", this);
    turnLabel->setAlignment(Qt::AlignCenter);
    turnLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #fab1a0; margin-bottom: 10px;");
    
    gameLayout->addWidget(opponentsArea);
    gameLayout->addWidget(turnLabel);
    gameLayout->addWidget(pilesArea);
    gameLayout->addWidget(handArea);
    
    auto* rightPanel = new QWidget(this);
    rightPanel->setFixedWidth(250);
    auto* rightLayout = new QVBoxLayout(rightPanel);
    
    setupChatUI(rightLayout);

    mainLayout->addLayout(gameLayout);
    mainLayout->addWidget(rightPanel);
    
    QTimer* gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, [this]() {
        if(gameClient && gameClient->IsInGame()) {
             gameClient->PollGameState();
             gameClient->PollChat();
        }
    });
    gameTimer->start(1000);
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

    // End Turn Button
    endTurnButton = new QPushButton("End Turn", this);
    endTurnButton->setStyleSheet("background-color: #0984e3; color: white; margin-top: 10px; font-weight: bold; padding: 8px; font-size: 14px;");
    layout->addWidget(endTurnButton);
    connect(endTurnButton, &QPushButton::clicked, [this](){
        if(gameClient) gameClient->EndTurn();
    });

    auto* debugWinButton = new QPushButton("Simulate Win (Debug)", this);
    debugWinButton->setStyleSheet("background-color: #d63031; color: white; margin-top: 10px;");
    layout->addWidget(debugWinButton);
    connect(debugWinButton, &QPushButton::clicked, [this](){
        if(gameClient) gameClient->ForceWin();
    });

    connect(sendChatButton, &QPushButton::clicked, this, &GameBoardWindow::onSendChatClicked);
    connect(chatInput, &QLineEdit::returnPressed, this, &GameBoardWindow::onSendChatClicked);
}

void GameBoardWindow::updateGameState(const GameState& state) {
    if (state.status == "finished") {

         static bool dialogShown = false;
         
         if (!this->findChild<GameOverDialog*>()) {
             auto* dialog = new GameOverDialog(state.won, state.deckSize, this);
             connect(dialog, &GameOverDialog::exitRequested, [this]() {
                  emit backToLobby();
             });
             connect(dialog, &GameOverDialog::restartRequested, [this]() {
                  emit backToLobby();
             });
             dialog->exec();
         }
         return; 
    }

  
    if (opponents.size() != state.otherPlayers.size()) {
        qDeleteAll(opponents);
        opponents.clear();
        QLayoutItem* item;
        while ((item = opponentsArea->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        
        for (const auto& p : state.otherPlayers) {
            auto* opp = new OpponentWidget(QString::fromStdString(p.name), this);
            opp->setCardCount(p.cardCount);
            opponentsArea->layout()->addWidget(opp);
            opponents.push_back(opp);
        }
    } else {
        for (size_t i=0; i<state.otherPlayers.size(); ++i) {
            opponents[i]->setName(QString::fromStdString(state.otherPlayers[i].name));
            opponents[i]->setCardCount(state.otherPlayers[i].cardCount);
        }
    }

    if(state.piles.size() == 4) {
        for(size_t i=0; i<4 && i<piles.size(); ++i) {
            piles[i]->setTopCardValue(state.piles[i].topCardValue);
        }
    }
    
    if(cardsRemainingLabel) {
        cardsRemainingLabel->setText(QString::number(state.deckSize));
    }
    
    qDeleteAll(handCards);
    handCards.clear();
    
    if(handArea->layout()) {
        QLayoutItem* item;
        while ((item = handArea->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    for (const auto& c : state.hand) {
        auto* card = new CardWidget(c.value, this);
        connect(card, &CardWidget::clicked, this, &GameBoardWindow::onCardClicked);
        
        if(c.value == selectedCardValue) {
            card->setSelected(true);
        }
        
        handArea->layout()->addWidget(card);
        handCards.push_back(card);
    }
    
    if(state.isMyTurn) {
        turnLabel->setText("RÂNDUL TĂU!");
        turnLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #2ecc71;"); // Green
        handArea->setEnabled(true);
        if(endTurnButton) {
            endTurnButton->setEnabled(true);
            endTurnButton->setStyleSheet("background-color: #0984e3; color: white; margin-top: 10px; font-weight: bold; padding: 8px; font-size: 14px;");
        }
    } else {
        turnLabel->setText(QString("Rândul lui %1").arg(QString::fromStdString(state.currentPlayerName)));
        turnLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #fab1a0;"); // Orange
        handArea->setEnabled(false);
        if(endTurnButton) {
            endTurnButton->setEnabled(false);
            endTurnButton->setStyleSheet("background-color: #636e72; color: #b2bec3; margin-top: 10px; font-weight: bold; padding: 8px; font-size: 14px;");
        }
    }
}



void GameBoardWindow::updateHandUI() {
}

void GameBoardWindow::onCardClicked(int value) {
    selectedCardValue = value;
    
    for (auto* card : handCards) {
        card->setSelected(card->getValue() == value);
    }
}

void GameBoardWindow::onPileClicked(PileType type) {
    if (selectedHandIndex != -1) {
    }
    
    CardWidget* selected = nullptr;
    for(auto* c : handCards) {
        if(c->isSelected()) {
            selected = c;
            break;
        }
    }
    
    if(selected) {
        int pileIdx = 0;
        switch(type) {
            case PileType::Ascending_1_to_99: 
                break;
        }
        
        PileWidget* senderPile = qobject_cast<PileWidget*>(sender());
        if(senderPile) {
            for(size_t i=0; i<piles.size(); ++i) {
                if(piles[i] == senderPile) {
                    pileIdx = i + 1;
                    break;
                }
            }
        }
        
        if(pileIdx > 0) {
            gameClient->PlayCard(selected->getValue(), pileIdx);
        }
    } else {
        QMessageBox::information(this, "Info", "Selectează o carte din mână!");
    }
}

void GameBoardWindow::onSendChatClicked() {
    QString msg = chatInput->text().trimmed();
    if (msg.isEmpty()) return;

    chatInput->clear();
    
    if (gameClient) {
        gameClient->SendChat(msg.toStdString());
    }
}

void GameBoardWindow::onChatUpdated(const std::vector<GameClient::ChatMessage>& messages) {
     for (const auto& msg : messages) {
         if (msg.id > lastMessageId) {
             
             QString senderName = QString("User %1").arg(msg.playerId);
             if (msg.playerId == gameClient->GetUserId()) senderName = currentUsername; 
             
             chatHistory->addItem(QString("[%1]: %2").arg(senderName, QString::fromStdString(msg.text)));
             lastMessageId = msg.id;
         }
     }
     if(chatHistory->count() > 0)
        chatHistory->scrollToBottom();
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

void GameBoardWindow::setupNotificationUI() {
    notificationLabel = new QLabel(this);
    notificationLabel->setAlignment(Qt::AlignCenter);
    notificationLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    notificationLabel->hide();
    
    notificationLabel->setFixedWidth(400);
    notificationLabel->setFixedHeight(60);
    notificationLabel->move(width()/2 - 200, 150); 
}

void GameBoardWindow::showNotification(const QString& message) {
    if(!notificationLabel) return;
    
    notificationLabel->setText(message);
    notificationLabel->adjustSize();
    
    int w = notificationLabel->width() + 40;
    int h = notificationLabel->height() + 20;
    notificationLabel->setFixedSize(w, h);
    notificationLabel->move((this->width() - w)/2, 80);
    
    notificationLabel->show();
    
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
