#include "OpponentWidget.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>

OpponentWidget::OpponentWidget(const QString& name, QWidget* parent)
    : QWidget(parent)
    , name(name)
{
    setFixedSize(120, 100);
    setupUI();
    updateStyle();
}

void OpponentWidget::setCardCount(int count) {
    cardCount = count;
    if (cardCountLabel) {
        cardCountLabel->setText(QString("%1 🂠").arg(cardCount));
    }
}

void OpponentWidget::setActive(bool active) {
    if (isActive != active) {
        isActive = active;
        updateStyle();
    }
}

void OpponentWidget::setupUI() {
    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    // Avatar Placeholder (Circle) + Name
    nameLabel = new QLabel(name, this);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setWordWrap(true);

    // Card Count
    cardCountLabel = new QLabel(QString("%1 🂠").arg(cardCount), this);
    cardCountLabel->setAlignment(Qt::AlignCenter);
    cardCountLabel->setStyleSheet("font-size: 18px; color: #fab1a0;");

    layout->addWidget(nameLabel);
    layout->addWidget(cardCountLabel);
}

void OpponentWidget::updateStyle() {
    QString bgColor = isActive ? "#2d3436" : "#2d3436"; // Darker bg
    QString borderColor = isActive ? "#00b894" : "#636e72"; // Green if active
    
    setStyleSheet(QString(
        "OpponentWidget {"
        "    background-color: %1;"
        "    border: 2px solid %2;"
        "    border-radius: 50px;" // Circle/Oval look
        "}"
        "QLabel {"
        "    color: #dfe6e9;"
        "    background-color: transparent;"
        "    font-weight: bold;"
        "}"
    ).arg(bgColor).arg(borderColor));
}

void OpponentWidget::paintEvent(QPaintEvent* event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
