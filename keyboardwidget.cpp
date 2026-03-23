#include "keyboardwidget.h"

#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>

KeyboardWidget::KeyboardWidget(QWidget *parent)
    : QWidget(parent), m_target(nullptr)
{
    QGridLayout *layout = new QGridLayout(this);

    QString keys[3][10] = {
        {"q","w","e","r","t","y","u","i","o","p"},
        {"a","s","d","f","g","h","j","k","l",""},
        {"z","x","c","v","b","n","m","","",""}
    };

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(keys[i][j] != "")
                createButton(keys[i][j], layout, i, j);
        }
    }

    QPushButton *spaceBtn = new QPushButton("Space", this);
    QPushButton *delBtn = new QPushButton("Del", this);

    connect(spaceBtn, &QPushButton::clicked, this, [=](){
        if(m_target) m_target->insertPlainText(" ");
    });

    connect(delBtn, &QPushButton::clicked, this, [=](){
        if(m_target) m_target->textCursor().deletePreviousChar();
    });

    layout->addWidget(spaceBtn, 3, 2, 1, 4);
    layout->addWidget(delBtn, 3, 6, 1, 2);
}

void KeyboardWidget::setTarget(QTextEdit *edit)
{
    m_target = edit;
}

void KeyboardWidget::createButton(const QString &text, QGridLayout *layout, int row, int col)
{
    QPushButton *btn = new QPushButton(text, this);
    btn->setMinimumSize(40, 40);

    connect(btn, &QPushButton::clicked, this, [=](){
        if(m_target)
            m_target->insertPlainText(text);
    });

    layout->addWidget(btn, row, col);
}
