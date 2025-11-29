#pragma once

/**
 * TEMPLATE_Window.h - Template pentru crearea unei ferestre noi
 * 
 * INSTRUCȚIUNI:
 * 1. Copiază acest fișier și redenumește-l la "YourWindowName.h"
 * 2. Înlocuiește toate aparițiile de "TEMPLATE_Window" cu "YourWindowName"
 * 3. Adaugă funcționalitățile specifice ferestrei tale
 * 4. Adaugă fereastra în WindowManager.h și WindowManager.cpp
 * 5. Vezi COMMIT_GUIDE.md pentru instrucțiuni detaliate
 */

#include <QWidget>
#include <QString>

class TEMPLATE_Window : public QWidget {
    Q_OBJECT

public:
    explicit TEMPLATE_Window(QWidget* parent = nullptr);
    ~TEMPLATE_Window();

    // Metode publice - adaugă metode specifice ferestrei tale
    void setUsername(const QString& username);

signals:
    // Adaugă semnale pentru navigare sau evenimente
    // Ex: void navigateToLobby();
    // Ex: void someEvent();

private slots:
    // Adaugă slots pentru butoane/evenimente
    // Ex: void onButtonClicked();

private:
    // Membri privati
    QString currentUsername;
    
    // Widget-uri UI - adaugă widget-uri specifice ferestrei tale
    // QPushButton* someButton;
    // QLabel* someLabel;
    
    // Metode private
    void setupUI();
    void applyStyles();
    void connectSignals();
};

