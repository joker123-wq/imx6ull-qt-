#include "systeminfopage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDateTime>
#include <QProcess>

SystemInfoPage::SystemInfoPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("系统信息", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    QPushButton *refreshBtn = new QPushButton("刷新信息", this);
    refreshBtn->setMinimumHeight(46);
    refreshBtn->setStyleSheet("QPushButton { background:#4C8DFF; color:white; border-radius:10px; font-size:18px; font-weight:bold; }");

    QString style = "font-size: 18px; color: #22313F;";
    m_kernelLabel = new QLabel("内核版本：--", this);
    m_cpuLabel = new QLabel("CPU信息：--", this);
    m_memLabel = new QLabel("内存信息：--", this);
    m_uptimeLabel = new QLabel("运行时间：--", this);
    m_ipLabel = new QLabel("IP地址：--", this);
    m_statusLabel = new QLabel("状态：未刷新", this);

    m_kernelLabel->setStyleSheet(style);
    m_cpuLabel->setStyleSheet(style);
    m_memLabel->setStyleSheet(style);
    m_uptimeLabel->setStyleSheet(style);
    m_ipLabel->setStyleSheet(style);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    connect(refreshBtn, &QPushButton::clicked, this, &SystemInfoPage::refreshInfo);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SystemInfoPage::refreshInfo);
    m_timer->start(3000);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(14);
    layout->addWidget(title);
    layout->addWidget(refreshBtn);
    layout->addWidget(m_kernelLabel);
    layout->addWidget(m_cpuLabel);
    layout->addWidget(m_memLabel);
    layout->addWidget(m_uptimeLabel);
    layout->addWidget(m_ipLabel);
    layout->addWidget(m_statusLabel);
    layout->addStretch();

    refreshInfo();
}
QString SystemInfoPage::readTextFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";
    QString text = QString::fromUtf8(file.readAll()).trimmed();
    file.close();
    return text;
}

void SystemInfoPage::refreshInfo()
{
    QString kernel = readTextFile("/proc/version");
    QString cpu = readTextFile("/proc/cpuinfo");
    QString mem = readTextFile("/proc/meminfo");
    QString uptime = readTextFile("/proc/uptime");

    if (!kernel.isEmpty())
        m_kernelLabel->setText("内核版本：" + kernel.section('\n', 0, 0));

    if (!cpu.isEmpty())
    {
        QString line = cpu.contains("Hardware")
                ? cpu.split('\n').filter(QRegExp("Hardware|model name|Processor")).value(0)
                : cpu.section('\n', 0, 0);

        m_cpuLabel->setText("CPU信息：" + line);
    }

    if (!mem.isEmpty())
        m_memLabel->setText("内存信息：" + mem.section('\n', 0, 1).replace('\n', "  "));

    if (!uptime.isEmpty())
        m_uptimeLabel->setText("运行时间：" + uptime.section(' ', 0, 0) + " 秒");

    QProcess process;
    process.start("sh", QStringList() << "-c" << "ip route get 1 2>/dev/null | awk '{print $7}'");
    process.waitForFinished(500);
    QString ip = QString::fromUtf8(process.readAllStandardOutput()).trimmed();
    if (ip.isEmpty())
        ip = "未知";
    m_ipLabel->setText("IP地址：" + ip);

    m_statusLabel->setText("状态：刷新完成  时间：" + QDateTime::currentDateTime().toString("hh:mm:ss"));
}

