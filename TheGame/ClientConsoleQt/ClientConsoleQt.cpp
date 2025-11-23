#include "ClientConsoleQt.h"
#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "WindowManager.h"

ClientConsoleQt::ClientConsoleQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle("The Game");
    setMinimumSize(800, 600);
    resize(800, 600);
    
    // Set WindowManager as central widget
    WindowManager* windowManager = new WindowManager(this);
    setCentralWidget(windowManager);
    windowManager->showLoginWindow();
}

ClientConsoleQt::~ClientConsoleQt()
{
}

