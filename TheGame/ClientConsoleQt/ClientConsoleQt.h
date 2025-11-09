#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ClientConsoleQt.h"
#include "LoginWindow.h"

class ClientConsoleQt : public QMainWindow
{
    Q_OBJECT

public:
    ClientConsoleQt(QWidget *parent = nullptr);
    ~ClientConsoleQt();

private slots:
    void onLoginSuccessful(const QString& username);

private:
    Ui::ClientConsoleQtClass ui;
    LoginWindow* loginWindow;
    QString currentUsername;
    
    void showLoginWindow();
    void showMainWindow();
};

