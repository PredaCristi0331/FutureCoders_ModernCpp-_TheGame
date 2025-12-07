#include "ProfileWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>

ProfileWindow::ProfileWindow(QWidget* parent) 
    : QWidget(parent)
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
    // Mock stats generation based on username hash or random for now
    if (statsLabel) {
        statsLabel->setText(QString("Nivel: 5\nJocuri Câștigate: 12\nJocuri Pierdute: 3\nData Înregistrării: %1")
            .arg(QDate::currentDate().addDays(-30).toString("dd.MM.yyyy")));
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
