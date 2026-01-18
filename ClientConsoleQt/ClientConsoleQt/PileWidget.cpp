#include "PileWidget.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>

PileWidget::PileWidget(PileType type, QWidget* parent)
    : QWidget(parent)
    , type(type)
    , topCardValue(type == PileType::Ascending_1_to_99 ? 1 : 100)
{
    setFixedSize(100, 140);
    setCursor(Qt::PointingHandCursor);
    setupUI();
    updateStyle();
}

void PileWidget::setTopCardValue(int value) {
    topCardValue = value;
    if (valueLabel) {
        valueLabel->setText(QString::number(value));
    }
}

void PileWidget::setHighlighted(bool highlighted) {
    if (isHighlighted != highlighted) {
        isHighlighted = highlighted;
        updateStyle();
    }
}

void PileWidget::mousePressEvent(QMouseEvent* event) {
    emit clicked(type);
    QWidget::mousePressEvent(event);
}

void PileWidget::setupUI() {
    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    iconLabel = new QLabel(this);
    iconLabel->setAlignment(Qt::AlignCenter);
    QString arrow = (type == PileType::Ascending_1_to_99) ? "▲" : "▼";
    QString range = (type == PileType::Ascending_1_to_99) ? "1 -> 99" : "100 -> 2";
    iconLabel->setText(arrow + "\n" + range);

    valueLabel = new QLabel(QString::number(topCardValue), this);
    valueLabel->setAlignment(Qt::AlignCenter);
    
    layout->addWidget(iconLabel);
    layout->addSpacing(10);
    layout->addWidget(valueLabel);
}

void PileWidget::updateStyle() {
    QString bgColor = "#2e2e4e";
    QString borderColor = isHighlighted ? "#4ecdc4" : "#533483";
    QString textColor = "#eaeaea"; 

    if (type == PileType::Ascending_1_to_99) {
    } else {
    }

    setStyleSheet(QString(
        "PileWidget {"
        "    background-color: %1;"
        "    border: 3px dashed %2;"
        "    border-radius: 12px;"
        "}"
        "QLabel {"
        "    color: %3;"
        "    font-weight: bold;"
        "    background-color: transparent;"
        "    border: none;"
        "}"
    ).arg(bgColor).arg(borderColor).arg(textColor));
    
    if (valueLabel) {
        valueLabel->setStyleSheet("font-size: 28px; color: #fff;");
    }
    if (iconLabel) {
        iconLabel->setStyleSheet("font-size: 12px; color: #aaa;");
    }
}

void PileWidget::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
