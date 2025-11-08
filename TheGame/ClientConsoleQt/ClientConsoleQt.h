#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ClientConsoleQt.h"

class ClientConsoleQt : public QMainWindow
{
    Q_OBJECT

public:
    ClientConsoleQt(QWidget *parent = nullptr);
    ~ClientConsoleQt();

private:
    Ui::ClientConsoleQtClass ui;
};

