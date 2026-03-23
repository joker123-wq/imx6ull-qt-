#include "notepadpage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include "keyboardwidget.h"

NotepadPage::NotepadPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("记事本", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    m_textEdit = new QTextEdit(this);
    m_textEdit->setPlaceholderText("在这里输入笔记内容...");
    m_textEdit->setStyleSheet(
        "QTextEdit {"
        "background: white;"
        "border: 2px solid #D5DEE7;"
        "border-radius: 12px;"
        "font-size: 18px;"
        "padding: 12px;"
        "}"
    );

    m_keyboard = new KeyboardWidget(this);
    m_keyboard->setTarget(m_textEdit);

    QPushButton *saveBtn = new QPushButton("保存", this);
    QPushButton *clearBtn = new QPushButton("清空", this);
    saveBtn->setMinimumHeight(46);
    clearBtn->setMinimumHeight(46);

    saveBtn->setStyleSheet("QPushButton { background:#4C8DFF; color:white; border-radius:10px; font-size:18px; font-weight:bold; }");
    clearBtn->setStyleSheet("QPushButton { background:#AAB6C3; color:white; border-radius:10px; font-size:18px; font-weight:bold; }");

    m_statusLabel = new QLabel("默认保存到 ./notes/ 目录", this);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    connect(saveBtn, &QPushButton::clicked, this, &NotepadPage::saveText);
    connect(clearBtn, &QPushButton::clicked, this, &NotepadPage::clearText);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(clearBtn);
    btnLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 20, 28, 20);
    mainLayout->setSpacing(14);
    mainLayout->addWidget(title);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_textEdit, 1);
    mainLayout->addWidget(m_keyboard);
    mainLayout->addWidget(m_statusLabel);
}

void NotepadPage::clearText()
{
    m_textEdit->clear();
    m_statusLabel->setText("内容已清空");
}

void NotepadPage::saveText() {
    QDir dir;
    if (!dir.exists("notes"))
        dir.mkdir("notes");
    QString fileName = QString("notes/note_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << m_textEdit->toPlainText();
        file.close();
        m_statusLabel->setText("保存成功：" + fileName);
    } else {
        m_statusLabel->setText("保存失败");
    }
}
