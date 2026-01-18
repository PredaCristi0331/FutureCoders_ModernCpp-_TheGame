#pragma once

#include <QWidget>
#include <QLabel>
#include <QString>

class OpponentWidget : public QWidget {
    Q_OBJECT

public:
    explicit OpponentWidget(const QString& name, QWidget* parent = nullptr);
    ~OpponentWidget() override = default;

    void setCardCount(int count);
    void setName(const QString& name);
    void setActive(bool active);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString name;
    int cardCount{6};
    bool isActive{false};
    QLabel* nameLabel{nullptr};
    QLabel* cardCountLabel{nullptr};

    void setupUI();
    void updateStyle();
};
