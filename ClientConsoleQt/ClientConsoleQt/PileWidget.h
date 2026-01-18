#pragma once

#include <QWidget>
#include <QLabel>
#include <QString>

enum class PileType {
    Ascending_1_to_99,
    Descending_100_to_2
};

class PileWidget : public QWidget {
    Q_OBJECT

public:
    explicit PileWidget(PileType type, QWidget* parent = nullptr);
    ~PileWidget() override = default;

    void setTopCardValue(int value);
    int getTopCardValue() const { return topCardValue; }
    PileType getType() const { return type; }

    void setHighlighted(bool highlighted);

signals:
    void clicked(PileType type);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    PileType type;
    int topCardValue;
    bool isHighlighted{false};
    QLabel* valueLabel{nullptr};
    QLabel* iconLabel{nullptr};

    void setupUI();
    void updateStyle();
};
