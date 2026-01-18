#include "GameOverDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

GameOverDialog::GameOverDialog(bool victory, int cardsRemaining, QWidget* parent)
    : QDialog(parent)
    , isVictory(victory)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setupUI();
    applyStyles();
    
    QLabel* msgLabel = findChild<QLabel*>("msgLabel");
    if (msgLabel) {
        if (victory) {
            msgLabel->setText("Ați învins Jocul! Felicitări!");
        } else {
            msgLabel->setText(QString("Joc terminat. Cărți rămase: %1").arg(cardsRemaining));
        }
    }
}

void GameOverDialog::setupUI() {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    
    auto* titleLabel = new QLabel(isVictory ? "VICTORIE!" : "GAME OVER", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");
    
    auto* msgLabel = new QLabel("", this);
    msgLabel->setAlignment(Qt::AlignCenter);
    msgLabel->setObjectName("msgLabel");
    
    auto* btnLayout = new QHBoxLayout();
    
    auto* backBtn = new QPushButton("Înapoi la Lobby", this);
    connect(backBtn, &QPushButton::clicked, [this]() {
        emit exitRequested();
        accept();
    });
    
    btnLayout->addWidget(backBtn);
    layout->addWidget(titleLabel);
    layout->addWidget(msgLabel);
    layout->addLayout(btnLayout);
}

void GameOverDialog::applyStyles() {
    QString mainColor = isVictory ? "#2ecc71" : "#e74c3c";
    
    setStyleSheet(QString(
        "QDialog {"
        "    background-color: #2d3436;"
        "    border: 2px solid %1;"
        "    border-radius: 10px;"
        "}"
        "QLabel#titleLabel {"
        "    color: %1;"
        "    font-size: 32px;"
        "    font-weight: bold;"
        "}"
        "QLabel#msgLabel {"
        "    color: #dfe6e9;"
        "    font-size: 18px;"
        "    margin: 20px 0;"
        "}"
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 10px 20px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover { opacity: 0.8; }"
    ).arg(mainColor));
}
