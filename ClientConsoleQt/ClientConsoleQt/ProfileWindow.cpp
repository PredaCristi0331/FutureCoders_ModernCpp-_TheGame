#include "ProfileWindow.h"
#include "GameClient.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>

ProfileWindow::ProfileWindow(GameClient* client, QWidget* parent) 
    : QWidget(parent)
    , m_client(client)
{
    setupUI();
    applyStyles();
    connectSignals();
}

ProfileWindow::~ProfileWindow() = default;

void ProfileWindow::setUsername(const QString& username) {
    currentUsername = username;
    if (usernameLabel) {
        usernameLabel->setText(QString("Jucător: %1").arg(username));
    }
    if (statsLabel) {
        refreshStats();
    }
}

void ProfileWindow::refreshStats() {
    if (!m_client) return;

    // Fetch user profile from server
    // Need userId first. GameClient has m_userId after Login.
    int userId = m_client->GetUserId();
    auto profile = m_client->GetUserProfile(userId);

    if (profile) {
        // Calculate stars
        QString stars = "";
        for(int i=0; i<5; ++i) stars += (i < profile->performance_score ? "★" : "☆");
        
        statsLabel->setText(QString(
            "Ore Jucate: %1\n"
            "Scor Performanță: %2 (%3)\n"
            "Jocuri Jucate: %4\n"
            "Jocuri Câștigate: %5\n"
            "Jocuri Pierdute: %6"
        ).arg(QString::number(profile->hours_played, 'f', 1))
         .arg(profile->performance_score)
         .arg(stars)
         .arg(profile->games_played)
         .arg(profile->games_won)
         .arg(profile->games_lost)
        );
    } else {
        statsLabel->setText("Nu s-au putut încărca datele...");
    }
}

void ProfileWindow::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Title
    titleLabel = new QLabel("PROFIL JUCĂTOR", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(20);

    // User Info Container
    auto* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(15);

    usernameLabel = new QLabel("Jucător: -", this);
    usernameLabel->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(usernameLabel);

    statsLabel = new QLabel(this);
    statsLabel->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(statsLabel);

    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();

    // Buttons
    backButton = new QPushButton("Înapoi la Lobby", this);
    backButton->setMinimumHeight(45);
    backButton->setCursor(Qt::PointingHandCursor);
    
    // Centering the button
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
}

void ProfileWindow::applyStyles() {
    setStyleSheet(
        "QWidget {"
        "    background-color: #1a1a2e;"
        "}"
        "QLabel {"
        "    color: #eaeaea;"
        "    font-size: 16px;"
        "}"
        "QPushButton {"
        "    background-color: #e94560;" // Different color for back button
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #ff6b6b;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #c0394d;"
        "}"
    );

    if (titleLabel) {
        titleLabel->setStyleSheet(
            "color: #533483;"
            "font-size: 28px;"
            "font-weight: bold;"
            "background-color: transparent;"
        );
    }

    if (statsLabel) {
        statsLabel->setStyleSheet(
            "color: #a0a0a0;"
            "font-size: 14px;"
            "background-color: #16213e;"
            "padding: 15px;"
            "border-radius: 10px;"
            "border: 1px solid #0f3460;"
        );
    }
}

void ProfileWindow::connectSignals() {
    connect(backButton, &QPushButton::clicked, this, &ProfileWindow::onBackClicked);
}

void ProfileWindow::onBackClicked() {
    emit backToLobby();
}
