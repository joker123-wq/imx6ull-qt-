#ifndef SYSTEMINFOPAGE_H
#define SYSTEMINFOPAGE_H

#include <QWidget>

class QLabel;
class QTimer;

class SystemInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit SystemInfoPage(QWidget *parent = nullptr);

private slots:
    void refreshInfo();

private:
    QString readTextFile(const QString &path);

private:
    QLabel *m_kernelLabel;
    QLabel *m_cpuLabel;
    QLabel *m_memLabel;
    QLabel *m_uptimeLabel;
    QLabel *m_ipLabel;
    QLabel *m_statusLabel;
    QTimer *m_timer;
};

#endif // SYSTEMINFOPAGE_H
