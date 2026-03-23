#include "sensortestpage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QString>

#include <fcntl.h>
#include <unistd.h>

SensorTestPage::SensorTestPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("传感器测试", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    QLabel *tip = new QLabel("AP3216C + ICM20608 数值测试界面", this);
    tip->setStyleSheet("font-size: 16px; color: #5B6B79;");

    QPushButton *refreshBtn = new QPushButton("手动刷新", this);
    refreshBtn->setMinimumHeight(46);
    refreshBtn->setStyleSheet(
        "QPushButton {"
        "background:#4C8DFF;"
        "color:white;"
        "border-radius:12px;"
        "font-size:18px;"
        "font-weight:bold;"
        "}"
        "QPushButton:hover { background:#3C7CE8; }"
        "QPushButton:pressed { background:#2F6BD0; }"
    );

    QString labelStyle = "font-size: 18px; color: #22313F;";

    m_alsLabel  = new QLabel("ALS：--", this);
    m_psLabel   = new QLabel("PS：--", this);
    m_irLabel   = new QLabel("IR：--", this);

    m_gxLabel   = new QLabel("Gyro X：--", this);
    m_gyLabel   = new QLabel("Gyro Y：--", this);
    m_gzLabel   = new QLabel("Gyro Z：--", this);

    m_axLabel   = new QLabel("Accel X：--", this);
    m_ayLabel   = new QLabel("Accel Y：--", this);
    m_azLabel   = new QLabel("Accel Z：--", this);

    m_tempLabel = new QLabel("Temp：--", this);
    m_statusLabel = new QLabel("状态：未刷新", this);

    m_alsLabel->setStyleSheet(labelStyle);
    m_psLabel->setStyleSheet(labelStyle);
    m_irLabel->setStyleSheet(labelStyle);

    m_gxLabel->setStyleSheet(labelStyle);
    m_gyLabel->setStyleSheet(labelStyle);
    m_gzLabel->setStyleSheet(labelStyle);

    m_axLabel->setStyleSheet(labelStyle);
    m_ayLabel->setStyleSheet(labelStyle);
    m_azLabel->setStyleSheet(labelStyle);

    m_tempLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #D35400;");
    m_statusLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    connect(refreshBtn, &QPushButton::clicked, this, &SensorTestPage::refreshSensorValue);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SensorTestPage::refreshSensorValue);
    m_timer->start(1000);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 18, 28, 18);
    mainLayout->setSpacing(8);

    mainLayout->addWidget(title);
    mainLayout->addWidget(tip);
    mainLayout->addWidget(refreshBtn);

    mainLayout->addWidget(m_alsLabel);
    mainLayout->addWidget(m_psLabel);
    mainLayout->addWidget(m_irLabel);

    mainLayout->addWidget(m_gxLabel);
    mainLayout->addWidget(m_gyLabel);
    mainLayout->addWidget(m_gzLabel);

    mainLayout->addWidget(m_axLabel);
    mainLayout->addWidget(m_ayLabel);
    mainLayout->addWidget(m_azLabel);

    mainLayout->addWidget(m_tempLabel);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addStretch();

    refreshSensorValue();
}

QString SensorTestPage::readSysfsValue(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QString value = QString::fromUtf8(file.readAll()).trimmed();
    file.close();
    return value;
}

bool SensorTestPage::readIcm20608Data(QString &gx, QString &gy, QString &gz,
                                      QString &ax, QString &ay, QString &az,
                                      QString &tempText)
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

    float gyro_x_act  = (float)(databuf[0]) / 16.4f;
    float gyro_y_act  = (float)(databuf[1]) / 16.4f;
    float gyro_z_act  = (float)(databuf[2]) / 16.4f;

    float accel_x_act = (float)(databuf[3]) / 2048.0f;
    float accel_y_act = (float)(databuf[4]) / 2048.0f;
    float accel_z_act = (float)(databuf[5]) / 2048.0f;

    float temp_act    = ((float)(databuf[6]) - 25.0f) / 326.8f + 25.0f;

    gx = QString::number(gyro_x_act, 'f', 2) + " °/s";
    gy = QString::number(gyro_y_act, 'f', 2) + " °/s";
    gz = QString::number(gyro_z_act, 'f', 2) + " °/s";

    ax = QString::number(accel_x_act, 'f', 2);
    ay = QString::number(accel_y_act, 'f', 2);
    az = QString::number(accel_z_act, 'f', 2);

    tempText = QString::number(temp_act, 'f', 2) + " °C";

    return true;
}

void SensorTestPage::refreshSensorValue()
{
    QString als = readSysfsValue("/sys/class/misc/ap3216c/als");
    QString ps  = readSysfsValue("/sys/class/misc/ap3216c/ps");
    QString ir  = readSysfsValue("/sys/class/misc/ap3216c/ir");

    if (!als.isEmpty()) m_alsLabel->setText("ALS：" + als);
    if (!ps.isEmpty())  m_psLabel->setText("PS：" + ps);
    if (!ir.isEmpty())  m_irLabel->setText("IR：" + ir);

    QString gx, gy, gz, ax, ay, az, tempText;
    bool ok = readIcm20608Data(gx, gy, gz, ax, ay, az, tempText);

    if (ok)
    {
        m_gxLabel->setText("Gyro X：" + gx);
        m_gyLabel->setText("Gyro Y：" + gy);
        m_gzLabel->setText("Gyro Z：" + gz);

        m_axLabel->setText("Accel X：" + ax);
        m_ayLabel->setText("Accel Y：" + ay);
        m_azLabel->setText("Accel Z：" + az);

        m_tempLabel->setText("Temp：" + tempText);
    }

    QString now = QDateTime::currentDateTime().toString("hh:mm:ss");

    if ((!als.isEmpty() || !ps.isEmpty() || !ir.isEmpty()) && ok)
        m_statusLabel->setText("状态：传感器读取成功    时间：" + now);
    else if (!als.isEmpty() || !ps.isEmpty() || !ir.isEmpty() || ok)
        m_statusLabel->setText("状态：部分读取成功    时间：" + now);
    else
        m_statusLabel->setText("状态：读取失败，请检查节点和权限    时间：" + now);
}
