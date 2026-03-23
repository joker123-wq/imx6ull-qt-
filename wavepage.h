#ifndef WAVEPAGE_H
#define WAVEPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;
class QComboBox;
class QTimer;
class WaveWidget;

class WavePage : public QWidget
{
    Q_OBJECT

public:
    explicit WavePage(QWidget *parent = nullptr);

private slots:
    void startMonitor();
    void stopMonitor();
    void clearWave();
    void refreshWave();

private:
    QString readSysfsValue(const QString &path);
    bool readIcmTemp(float &tempValue);

private:
    QComboBox *m_channelBox;
    QPushButton *m_startBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_clearBtn;
    QLabel *m_valueLabel;
    QLabel *m_statusLabel;
    WaveWidget *m_waveWidget;
    QTimer *m_timer;
};

#endif // WAVEPAGE_H
