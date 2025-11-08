#include "ClientConsoleQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ClientConsoleQt window;
    window.show();
    return app.exec();
}
