#pragma once

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

class ProfileWindow : public QWidget {
    Q_OBJECT

public:
    explicit ProfileWindow(QWidget* parent = nullptr);
    ~ProfileWindow() override;

    void setUsername(const QString& username);

signals:
    void backToLobby();

private slots:
    void onBackClicked();

private:
    QString currentUsername;
    
    // UI Elements
    QLabel* titleLabel{nullptr};
    QLabel* usernameLabel{nullptr};
    QLabel* statsLabel{nullptr}; // Mock stats
    QPushButton* backButton{nullptr};
    
    void setupUI();
    void applyStyles();
    void connectSignals();
};
