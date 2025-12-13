#pragma once

#include <QWidget>
#include <QLabel>
#include <QString>

class CardWidget : public QWidget {
    Q_OBJECT

public:
    explicit CardWidget(int value, QWidget* parent = nullptr);
    ~CardWidget() override = default;

    void setValue(int value);
    int getValue() const { return m_value; }

    void setSelected(bool selected);
    bool isSelected() const { return m_isSelected; }

signals:
    void clicked(int value);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    int m_value;
    bool m_isSelected;
    QLabel* m_valueLabel{nullptr};

    void setupUI();
    void updateStyle();
};
