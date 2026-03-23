#ifndef SENSORTESTPAGE_H
#define SENSORTESTPAGE_H

#include <QWidget>

class QLabel;
class QTimer;

class SensorTestPage : public QWidget
{
    Q_OBJECT

public:
    explicit SensorTestPage(QWidget *parent = nullptr);

private slots:
    void refreshSensorValue();

private:
    QString readSysfsValue(const QString &path);
    bool readIcm20608Data(QString &gx, QString &gy, QString &gz,
                          QString &ax, QString &ay, QString &az,
                          QString &tempText);

private:
    QLabel *m_alsLabel;
    QLabel *m_psLabel;
    QLabel *m_irLabel;

    QLabel *m_gxLabel;
    QLabel *m_gyLabel;
    QLabel *m_gzLabel;

    QLabel *m_axLabel;
    QLabel *m_ayLabel;
    QLabel *m_azLabel;

    QLabel *m_tempLabel;
    QLabel *m_statusLabel;

    QTimer *m_timer;
};

#endif // SENSORTESTPAGE_H
