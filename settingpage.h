#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QWidget>

class QLabel;
class QSlider;
class QComboBox;

class SettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);

signals:
    void brightnessChanged(int value);
    void volumeChanged(int value);

private slots:
    void applyBrightness(int value);
    void applyVolume(int value);
    void saveTheme(const QString &theme);

private:
    bool writeSysfs(const QString &path, const QString &value);
    int readBrightnessValue() const;
    int brightnessToSlider(int hwValue) const;
    int sliderToBrightness(int sliderValue) const;

private:
    QLabel *m_brightnessLabel;
    QLabel *m_volumeLabel;
    QLabel *m_statusLabel;
    QSlider *m_brightnessSlider;
    QSlider *m_volumeSlider;
    QComboBox *m_themeBox;
};

#endif // SETTINGPAGE_H
