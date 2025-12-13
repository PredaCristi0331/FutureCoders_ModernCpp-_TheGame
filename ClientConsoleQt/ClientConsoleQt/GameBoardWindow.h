#pragma once

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <vector>

class GameClient;
class CardWidget;

class GameBoardWindow : public QWidget {
    Q_OBJECT

public:
    explicit GameBoardWindow(GameClient* client, QWidget* parent = nullptr);
    ~GameBoardWindow() override;

    void setUsername(const QString& username);

private slots:
    void onSendChatClicked();
    void onCardClicked(int value);

private:
    GameClient* gameClient;
    QString currentUsername;
    int selectedHandIndex{-1};

    // UI Areas
    QWidget* opponentsArea{nullptr};
    QWidget* pilesArea{nullptr};
    QWidget* handArea{nullptr};
    
    // Chat UI (Commit 3)
    QListWidget* chatHistory{nullptr};
    QLineEdit* chatInput{nullptr};
    QPushButton* sendChatButton{nullptr};

    // Hand Widgets
    std::vector<CardWidget*> handCards;

    void setupUI();
    void setupChatUI(QVBoxLayout* rightLayout);
    void applyStyles();
    void updateHandUI(); // Mock for now
};
