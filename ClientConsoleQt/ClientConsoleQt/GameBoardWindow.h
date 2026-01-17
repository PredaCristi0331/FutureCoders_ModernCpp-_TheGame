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
class GameClient;
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

private slots:
    void updateGameState(const GameState& state);
    void onSendChatClicked();
    void onCardClicked(int value);
    void onPileClicked(PileType type);

private:
    GameClient* gameClient;
    QString currentUsername;
    int selectedHandIndex{-1};

    // UI Areas
    QWidget* opponentsArea{nullptr};
    QWidget* pilesArea{nullptr};
    QWidget* handArea{nullptr};
    std::vector<CardWidget*> handCards;
    
    // Chat UI (Commit 3)
    QListWidget* chatHistory{nullptr};
    QLineEdit* chatInput{nullptr};
    QPushButton* sendChatButton{nullptr};

    // Piles
    std::vector<PileWidget*> piles;
    QLabel* deckLabel{nullptr};
    QLabel* cardsRemainingLabel{nullptr};

    // Opponents
    std::vector<OpponentWidget*> opponents;

    void setupUI();
    void setupChatUI(QVBoxLayout* rightLayout);
    void applyStyles();
    void updateHandUI(); // Mock for now

    // Inline Commit 3: Notification Logic
    QLabel* turnLabel{nullptr};
    QLabel* notificationLabel{nullptr};
    void showNotification(const QString& message);
    void setupNotificationUI();
};
