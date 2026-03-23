#ifndef LEDTESTPAGE_H
#define LEDTESTPAGE_H

#include <QWidget>

class QLabel;

class LedTestPage : public QWidget
{
    Q_OBJECT

public:
    explicit LedTestPage(QWidget *parent = nullptr);

private slots:
    void ledOn();
    void ledOff();

private:
    bool writeSysfs(const QString &path, const QString &value);

private:
    QLabel *m_statusLabel;
};

#endif // LEDTESTPAGE_H
