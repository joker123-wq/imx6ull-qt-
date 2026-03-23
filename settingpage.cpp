#include "settingpage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QFile>
#include <QProcess>
#include <QtGlobal>

SettingPage::SettingPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("设置", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    int currentHwBrightness = readBrightnessValue();
    int currentSliderBrightness = brightnessToSlider(currentHwBrightness);

    m_brightnessLabel = new QLabel(QString("亮度等级：%1").arg(currentSliderBrightness), this);
    m_volumeLabel = new QLabel("音量：80", this);
    m_statusLabel = new QLabel("状态：待调整", this);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    // UI 亮度只做 5 档：0~4，对应真实背光 0,2,4,6,7
    m_brightnessSlider = new QSlider(Qt::Horizontal, this);
    m_brightnessSlider->setRange(0, 4);
    m_brightnessSlider->setValue(currentSliderBrightness);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(80);

    m_themeBox = new QComboBox(this);
    m_themeBox->addItem("浅色");
    m_themeBox->addItem("深色");

    connect(m_brightnessSlider, &QSlider::valueChanged, this, &SettingPage::applyBrightness);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &SettingPage::applyVolume);
    connect(m_themeBox, &QComboBox::currentTextChanged, this, &SettingPage::saveTheme);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(16);
    layout->addWidget(title);
    layout->addWidget(m_brightnessLabel);
    layout->addWidget(m_brightnessSlider);
    layout->addWidget(m_volumeLabel);
    layout->addWidget(m_volumeSlider);
    layout->addWidget(new QLabel("主题选择：", this));
    layout->addWidget(m_themeBox);
    layout->addWidget(m_statusLabel);
    layout->addStretch();
}

bool SettingPage::writeSysfs(const QString &path, const QString &value)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    file.write(value.toUtf8());
    file.close();
    return true;
}

int SettingPage::readBrightnessValue() const
{
#ifdef Q_OS_LINUX
    QFile file("/sys/class/backlight/backlight/brightness");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString value = QString::fromUtf8(file.readAll()).trimmed();
        file.close();

        bool ok = false;
        int brightness = value.toInt(&ok);
        if (ok)
            return brightness;
    }
#endif
    return 7;
}

int SettingPage::brightnessToSlider(int hwValue) const
{
    if (hwValue <= 0) return 0;
    if (hwValue <= 2) return 1;
    if (hwValue <= 4) return 2;
    if (hwValue <= 6) return 3;
    return 4;
}

int SettingPage::sliderToBrightness(int sliderValue) const
{
    static const int levelMap[5] = {0, 2, 4, 6, 7};
    if (sliderValue < 0) sliderValue = 0;
    if (sliderValue > 4) sliderValue = 4;
    return levelMap[sliderValue];
}

void SettingPage::applyBrightness(int value)
{
    int hwValue = sliderToBrightness(value);
    m_brightnessLabel->setText(QString("亮度等级：%1").arg(value));

#ifdef Q_OS_LINUX
    bool ok = writeSysfs("/sys/class/backlight/backlight/brightness",
                         QString::number(hwValue));

    if (ok)
        m_statusLabel->setText(QString("状态：亮度已设置为 档位%1 (实际值%2)").arg(value).arg(hwValue));
    else
        m_statusLabel->setText("状态：亮度设置失败");
#else
    m_statusLabel->setText(QString("状态：Windows下仅模拟亮度 档位%1").arg(value));
#endif

    emit brightnessChanged(value);
}

void SettingPage::applyVolume(int value)
{
    m_volumeLabel->setText(QString("音量：%1").arg(value));

#ifdef Q_OS_LINUX
    int alsaValue = value * 255 / 100;
    QString cmd = QString("amixer set Playback %1").arg(alsaValue);
    int ret = QProcess::execute("sh", QStringList() << "-c" << cmd);

    if (ret == 0)
        m_statusLabel->setText(QString("状态：音量已设置为 %1").arg(value));
    else
        m_statusLabel->setText("状态：音量设置失败");
#else
    m_statusLabel->setText(QString("状态：Windows下仅模拟音量 %1").arg(value));
#endif

    emit volumeChanged(value);
}

void SettingPage::saveTheme(const QString &theme)
{
    m_statusLabel->setText("状态：主题已切换为 " + theme);
}
