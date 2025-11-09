#include "ClientConsoleQt.h"
#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    ClientConsoleQt window;
    window.setWindowTitle("The Game");
    window.show();
    window.raise();
    window.activateWindow();
    
    return app.exec();
}
