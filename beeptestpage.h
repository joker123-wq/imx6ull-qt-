#ifndef BEEPTESTPAGE_H
#define BEEPTESTPAGE_H

#include <QWidget>

class QLabel;

class BeepTestPage : public QWidget
{
    Q_OBJECT

public:
    explicit BeepTestPage(QWidget *parent = nullptr);

private slots:
    void beepOn();
    void beepOff();

private:
    bool writeSysfs(const QString &path, const QString &value);

private:
    QLabel *m_statusLabel;
};

#endif // BEEPTESTPAGE_H
