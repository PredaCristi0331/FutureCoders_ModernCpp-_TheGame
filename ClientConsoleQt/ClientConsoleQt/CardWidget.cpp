#include "CardWidget.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>

CardWidget::CardWidget(int value, QWidget* parent)
    : QWidget(parent)
    , m_value(value)
    , m_isSelected(false)
{
    setFixedSize(80, 120); // Standard card size
    setCursor(Qt::PointingHandCursor);
    setupUI();
    updateStyle();
}

void CardWidget::setValue(int value) {
    m_value = value;
    if (m_valueLabel) {
        m_valueLabel->setText(QString::number(m_value));
    }
}

void CardWidget::setSelected(bool selected) {
    if (m_isSelected != selected) {
        m_isSelected = selected;
        updateStyle();
    }
}

void CardWidget::mousePressEvent(QMouseEvent* event) {
    emit clicked(m_value);
    QWidget::mousePressEvent(event);
}

void CardWidget::setupUI() {
    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    m_valueLabel = new QLabel(QString::number(m_value), this);
    m_valueLabel->setAlignment(Qt::AlignCenter);
    UI: // Small fix for label alignment if needed
    
    layout->addWidget(m_valueLabel);
}

void CardWidget::updateStyle() {
    // Dynamic styling based on selection
    QString borderColor = m_isSelected ? "#e94560" : "#0f3460";
    QString bgColor = m_isSelected ? "#fcdab7" : "#eaeaea"; // Highlight vs Normal
    QString textColor = "#1a1a2e";

    setStyleSheet(QString(
        "CardWidget {"
        "    background-color: %1;"
        "    border: 3px solid %2;"
        "    border-radius: 10px;"
        "}"
        "QLabel {"
        "    color: %3;"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "    background-color: transparent;"
        "    border: none;"
        "}"
    ).arg(bgColor).arg(borderColor).arg(textColor));
}

void CardWidget::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
