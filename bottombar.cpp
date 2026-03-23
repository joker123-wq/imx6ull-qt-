#include "bottombar.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QStringList>
#include <QDebug>

BottomBar::BottomBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(70);
    setStyleSheet(
        "QWidget {"
        "background-color: #1F2A36;"
        "border-top: 1px solid #32414F;"
        "}"
    );

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(30);

    QStringList texts;
    texts << "返回" << "主页" << "任务" << "退出";

    for (const QString &text : texts)
    {
        QPushButton *btn = new QPushButton(text, this);
        btn->setMinimumSize(110, 45);
        btn->setStyleSheet(
            "QPushButton {"
            "background-color: #32414F;"
            "color: white;"
            "border-radius: 10px;"
            "font-size: 18px;"
            "font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "background-color: #435567;"
            "}"
            "QPushButton:pressed {"
            "background-color: #566C80;"
            "}"
        );

        connect(btn, &QPushButton::clicked, this, [=]() {
            qDebug() << "底部按钮:" << text;
            if (text == "退出")
                QApplication::quit();
            else if (text == "主页")
                emit homeClicked();
            else if (text == "返回")
                emit backClicked();
        });

        layout->addWidget(btn);
    }

    layout->addStretch();
}
