#pragma once

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <vector>

#include "GameState.h"
#include "GameClient.h"
class CardWidget;
class PileWidget;
#include "PileWidget.h"
class OpponentWidget;
#include "OpponentWidget.h"

class GameBoardWindow : public QWidget {
    Q_OBJECT

public:
    explicit GameBoardWindow(GameClient* client, QWidget* parent = nullptr);
    ~GameBoardWindow() override;

    void setUsername(const QString& username);

signals:
    void backToLobby();

private slots:
    void updateGameState(const GameState& state);
    void onSendChatClicked();
    void onCardClicked(int value);
    void onPileClicked(PileType type);
    void onChatUpdated(const std::vector<GameClient::ChatMessage>& messages);

private:
    GameClient* gameClient;
    QString currentUsername;
    int selectedHandIndex{-1};
    int selectedCardValue{-1}; 
    int lastMessageId = 0;

    QWidget* opponentsArea{nullptr};
    QWidget* pilesArea{nullptr};
    QWidget* handArea{nullptr};
    std::vector<CardWidget*> handCards;
    
    QListWidget* chatHistory{nullptr};
    QLineEdit* chatInput{nullptr};
    QPushButton* sendChatButton{nullptr};

    std::vector<PileWidget*> piles;
    QLabel* deckLabel{nullptr};
    QLabel* cardsRemainingLabel{nullptr};
    QPushButton* endTurnButton{nullptr};

    std::vector<OpponentWidget*> opponents;

    void setupUI();
    void setupChatUI(QVBoxLayout* rightLayout);
    void applyStyles();
    void updateHandUI();

    QLabel* turnLabel{nullptr};
    QLabel* notificationLabel{nullptr};
    void showNotification(const QString& message);
    void setupNotificationUI();
};
