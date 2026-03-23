#include "titlebar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QStorageInfo>

TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent),
      m_volumeValue(80),
      m_brightnessValue(7)
{
    setFixedHeight(60);
    setStyleSheet(
        "QWidget {"
        "background-color: #1F2A36;"
        "color: white;"
        "border-bottom: 1px solid #32414F;"
        "}"
    );

    m_titleLabel = new QLabel("SmartPad", this);
    m_titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    m_timeLabel = new QLabel(this);
    m_timeLabel->setStyleSheet("font-size: 18px;");
    m_timeLabel->setAlignment(Qt::AlignCenter);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("font-size: 16px;");
    m_statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(15);
    layout->addWidget(m_titleLabel, 1);
    layout->addWidget(m_timeLabel, 1);
    layout->addWidget(m_statusLabel, 1);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [=]() {
        updateTime();
        updateStatus();
    });
    m_timer->start(1000);

    updateTime();
    updateStatus();
}

void TitleBar::setVolumeValue(int value)
{
    m_volumeValue = value;
    updateStatus();
}

void TitleBar::setBrightnessValue(int value)
{
    m_brightnessValue = value;
    updateStatus();
}

void TitleBar::updateTime()
{
    m_timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm:ss"));
}

void TitleBar::updateStatus()
{
    QStorageInfo storage = QStorageInfo::root();
    double freeGB = storage.bytesAvailable() / 1024.0 / 1024.0 / 1024.0;

    m_statusLabel->setText(
        QString("WiFi  音量:%1%%  亮度:%2  存储:%3GB")
            .arg(m_volumeValue)
            .arg(m_brightnessValue)
            .arg(QString::number(freeGB, 'f', 1))
    );
}
