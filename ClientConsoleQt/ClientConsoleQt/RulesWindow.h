#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class RulesWindow : public QDialog {
    Q_OBJECT

public:
    explicit RulesWindow(QWidget* parent = nullptr);

private:
    void setupUI();
    void applyStyles();
};
