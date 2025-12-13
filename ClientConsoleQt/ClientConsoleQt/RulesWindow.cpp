#include "RulesWindow.h"
#include <QVBoxLayout>
#include <QTextEdit>

RulesWindow::RulesWindow(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Regulamentul Jocului - The Game");
    setMinimumSize(500, 400);
    setupUI();
    applyStyles();
}

void RulesWindow::setupUI() {
    auto* layout = new QVBoxLayout(this);
    
    auto* title = new QLabel("Cum se joacă?", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #533483;");
    
    auto* rulesText = new QTextEdit(this);
    rulesText->setReadOnly(true);
    rulesText->setHtml(
        "<h3>Obiectiv</h3>"
        "<p>Jucați toate cele 98 de cărți din pachet pe cele 4 teancuri.</p>"
        "<h3>Reguli</h3>"
        "<ul>"
        "<li>Există 2 teancuri crescătoare (1 -> 99) și 2 descrescătoare (100 -> 2).</li>"
        "<li>Fiecare jucător trebuie să joace minim 2 cărți pe tură.</li>"
        "<li>Puteți juca o carte cu diferența exact 10 în sens invers pentru a 'salva' un teanc.</li>"
        "<li>Comunicați cu coechipierii, dar nu dezvăluiți valori exacte!</li>"
        "</ul>"
    );
    
    auto* closeBtn = new QPushButton("Închide", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    
    layout->addWidget(title);
    layout->addWidget(rulesText);
    layout->addWidget(closeBtn);
}

void RulesWindow::applyStyles() {
    setStyleSheet(
        "QDialog {"
        "    background-color: #f0f0f0;"
        "}"
        "QTextEdit {"
        "    background-color: white;"
        "    border: 1px solid #ccc;"
        "    font-size: 14px;"
        "    padding: 10px;"
        "}"
        "QPushButton {"
        "    background-color: #533483;"
        "    color: white;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #6a4c93; }"
    );
}
