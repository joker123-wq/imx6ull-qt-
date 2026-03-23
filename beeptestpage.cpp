#include "beeptestpage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>

static const QString BEEP_PATH = "/sys/class/leds/beep/brightness";

BeepTestPage::BeepTestPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("BEEP测试", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    QLabel *tip = new QLabel("先把 BEEP 的 sysfs 节点路径改成你板子实际路径", this);
    tip->setStyleSheet("font-size: 16px; color: #5B6B79;");

    QPushButton *onBtn = new QPushButton("打开蜂鸣器", this);
    QPushButton *offBtn = new QPushButton("关闭蜂鸣器", this);
    onBtn->setMinimumHeight(52);
    offBtn->setMinimumHeight(52);

    onBtn->setStyleSheet("QPushButton { background:#F59E0B; color:white; border-radius:12px; font-size:20px; font-weight:bold; }");
    offBtn->setStyleSheet("QPushButton { background:#6B7280; color:white; border-radius:12px; font-size:20px; font-weight:bold; }");

    m_statusLabel = new QLabel("当前节点：" + BEEP_PATH, this);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #22313F;");

    connect(onBtn, &QPushButton::clicked, this, &BeepTestPage::beepOn);
    connect(offBtn, &QPushButton::clicked, this, &BeepTestPage::beepOff);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(onBtn);
    btnLayout->addWidget(offBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 20, 28, 20);
    mainLayout->setSpacing(18);
    mainLayout->addWidget(title);
    mainLayout->addWidget(tip);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addStretch();
}

bool BeepTestPage::writeSysfs(const QString &path, const QString &value)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    file.write(value.toUtf8());
    file.close();
    return true;
}

void BeepTestPage::beepOn()
{
    if (writeSysfs(BEEP_PATH, "1"))
        m_statusLabel->setText("蜂鸣器已打开");
    else
        m_statusLabel->setText("蜂鸣器控制失败，请检查节点路径和权限");
}

void BeepTestPage::beepOff()
{
    if (writeSysfs(BEEP_PATH, "0"))
        m_statusLabel->setText("蜂鸣器已关闭");
    else
        m_statusLabel->setText("蜂鸣器控制失败，请检查节点路径和权限");
}
