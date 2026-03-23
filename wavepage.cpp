#include "wavepage.h"
#include "wavewidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
#include <QFile>
#include <QDateTime>
#include <fcntl.h>
#include <unistd.h>

WavePage::WavePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("波形监视", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    m_channelBox = new QComboBox(this);
    m_channelBox->addItem("温度 Temp");
    m_channelBox->addItem("光照 ALS");

    m_startBtn = new QPushButton("开始", this);
    m_stopBtn = new QPushButton("停止", this);
    m_clearBtn = new QPushButton("清空", this);

    m_valueLabel = new QLabel("当前值：--", this);
    m_statusLabel = new QLabel("状态：未启动", this);

    m_waveWidget = new WaveWidget(this);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WavePage::refreshWave);

    connect(m_startBtn, &QPushButton::clicked, this, &WavePage::startMonitor);
    connect(m_stopBtn, &QPushButton::clicked, this, &WavePage::stopMonitor);
    connect(m_clearBtn, &QPushButton::clicked, this, &WavePage::clearWave);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(new QLabel("通道：", this));
    topLayout->addWidget(m_channelBox);
    topLayout->addWidget(m_startBtn);
    topLayout->addWidget(m_stopBtn);
    topLayout->addWidget(m_clearBtn);
    topLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 20, 28, 20);
    mainLayout->setSpacing(12);
    mainLayout->addWidget(title);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_valueLabel);
    mainLayout->addWidget(m_waveWidget, 1);
    mainLayout->addWidget(m_statusLabel);
}

QString WavePage::readSysfsValue(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QString value = QString::fromUtf8(file.readAll()).trimmed();
    file.close();
    return value;
}

bool WavePage::readIcmTemp(float &tempValue)
{
    const char *filename = "/dev/icm20608";
    signed int databuf[7] = {0};

    int fd = ::open(filename, O_RDWR);
    if (fd < 0)
        return false;

    int ret = ::read(fd, databuf, sizeof(databuf));
    ::close(fd);

    if (ret != 0)
        return false;

    tempValue = ((float)(databuf[6]) - 25.0f) / 326.8f + 25.0f;
    return true;
}

void WavePage::startMonitor()
{
    m_timer->start(500);
    m_statusLabel->setText("状态：采集中");
}

void WavePage::stopMonitor()
{
    m_timer->stop();
    m_statusLabel->setText("状态：已停止");
}

void WavePage::clearWave()
{
    m_waveWidget->clearData();
    m_valueLabel->setText("当前值：--");
    m_statusLabel->setText("状态：已清空");
}

void WavePage::refreshWave()
{
    QString channel = m_channelBox->currentText();

    if (channel.contains("Temp"))
    {
        float tempValue = 0.0f;
        if (readIcmTemp(tempValue))
        {
            m_waveWidget->addPoint(tempValue);
            m_valueLabel->setText(QString("当前值：%1 °C").arg(tempValue, 0, 'f', 2));
            m_statusLabel->setText("状态：温度采集中  时间：" + QDateTime::currentDateTime().toString("hh:mm:ss"));
        }
        else
        {
            m_statusLabel->setText("状态：温度读取失败");
        }
    }
    else if (channel.contains("ALS"))
    {
        QString als = readSysfsValue("/sys/class/misc/ap3216c/als");
        if (!als.isEmpty())
        {
            float value = als.toFloat();
            m_waveWidget->addPoint(value);
            m_valueLabel->setText(QString("当前值：%1").arg(value, 0, 'f', 0));
            m_statusLabel->setText("状态：光照采集中  时间：" + QDateTime::currentDateTime().toString("hh:mm:ss"));
        }
        else
        {
            m_statusLabel->setText("状态：ALS读取失败");
        }
    }
}
