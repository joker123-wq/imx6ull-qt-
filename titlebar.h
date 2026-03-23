#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class QLabel;
class QTimer;

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);

public slots:
    void setVolumeValue(int value);
    void setBrightnessValue(int value);

private:
    void updateTime();
    void updateStatus();

private:
    QLabel *m_titleLabel;
    QLabel *m_timeLabel;
    QLabel *m_statusLabel;
    QTimer *m_timer;

    int m_volumeValue;
    int m_brightnessValue;
};

#endif // TITLEBAR_H
