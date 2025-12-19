#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class GameOverDialog : public QDialog {
    Q_OBJECT

public:
    explicit GameOverDialog(bool victory, int cardsRemaining, QWidget* parent = nullptr);

signals:
    void restartRequested();
    void exitRequested();

private:
    bool isVictory;
    void setupUI();
    void applyStyles();
};
