#include "ledtestpage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>

static const QString LED_PATH = "/sys/class/leds/sys-led/brightness";

LedTestPage::LedTestPage(QWidget *parent)
    : QWidget(parent)
{
    QFile triggerFile("/sys/class/leds/sys-led/trigger");
    if (triggerFile.open(QIODevice::WriteOnly))
    {
        triggerFile.write("none");
        triggerFile.close();
    }

    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("LED测试", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    QLabel *tip = new QLabel("当前使用板载 LED 节点进行控制", this);
    tip->setStyleSheet("font-size: 16px; color: #5B6B79;");

    QPushButton *onBtn = new QPushButton("点亮LED", this);
    QPushButton *offBtn = new QPushButton("熄灭LED", this);
    onBtn->setMinimumHeight(52);
    offBtn->setMinimumHeight(52);

    onBtn->setStyleSheet("QPushButton { background:#42B883; color:white; border-radius:12px; font-size:20px; font-weight:bold; }");
    offBtn->setStyleSheet("QPushButton { background:#FF6B6B; color:white; border-radius:12px; font-size:20px; font-weight:bold; }");

    m_statusLabel = new QLabel("当前节点：" + LED_PATH, this);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #22313F;");

    connect(onBtn, &QPushButton::clicked, this, &LedTestPage::ledOn);
    connect(offBtn, &QPushButton::clicked, this, &LedTestPage::ledOff);

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

bool LedTestPage::writeSysfs(const QString &path, const QString &value)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    file.write(value.toUtf8());
    file.close();
    return true;
}

void LedTestPage::ledOn()
{
    if (writeSysfs(LED_PATH, "1"))
        m_statusLabel->setText("LED 已点亮");
    else
        m_statusLabel->setText("LED 控制失败，请检查节点路径和权限");
}

void LedTestPage::ledOff()
{
    if (writeSysfs(LED_PATH, "0"))
        m_statusLabel->setText("LED 已熄灭");
    else
        m_statusLabel->setText("LED 控制失败，请检查节点路径和权限");
}
