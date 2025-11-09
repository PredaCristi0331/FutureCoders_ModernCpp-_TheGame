#include "ClientConsoleQt.h"
#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>

ClientConsoleQt::ClientConsoleQt(QWidget *parent)
    : QMainWindow(parent)
    , loginWindow(nullptr)
{
    ui.setupUi(this);
    setWindowTitle("The Game");
    setMinimumSize(800, 600);
    resize(800, 600);
    
    // Elimină widget-ul central existent din UI
    QWidget* oldCentralWidget = takeCentralWidget();
    if (oldCentralWidget) {
        oldCentralWidget->deleteLater();
    }
    
    // Afișează LoginWindow la start
    showLoginWindow();
}

ClientConsoleQt::~ClientConsoleQt()
{
    // LoginWindow va fi șters automat de Qt când parent-ul este distrus
}

void ClientConsoleQt::showLoginWindow() {
    // Creează și afișează LoginWindow ca widget central
    if (!loginWindow) {
        loginWindow = new LoginWindow(this);
        connect(loginWindow, &LoginWindow::loginSuccessful, 
                this, &ClientConsoleQt::onLoginSuccessful);
    }
    
    // Setează LoginWindow ca widget central
    setCentralWidget(loginWindow);
    loginWindow->show();
    loginWindow->setFocus();
}

void ClientConsoleQt::onLoginSuccessful(const QString& username) {
    currentUsername = username;
    
    // Ascunde LoginWindow
    if (loginWindow) {
        loginWindow->hide();
        loginWindow->setParent(nullptr); // Elimină parent-ul temporar
    }
    
    // Afișează fereastra principală
    showMainWindow();
}

void ClientConsoleQt::showMainWindow() {
    // Creează un widget nou pentru conținutul principal
    QWidget* mainWidget = new QWidget(this);
    
    // Setează un mesaj temporar (va fi înlocuit cu UI-ul real în commit-urile următoare)
    QLabel* welcomeLabel = new QLabel(QString("Bine ai venit, %1!").arg(currentUsername), mainWidget);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 24px; color: #533483; padding: 50px;");
    
    QVBoxLayout* layout = new QVBoxLayout(mainWidget);
    layout->addWidget(welcomeLabel);
    layout->addStretch();
    
    // Setează widget-ul principal ca central widget
    setCentralWidget(mainWidget);
    mainWidget->show();
}

