#ifndef SPLASHPAGE_H
#define SPLASHPAGE_H

#include <QWidget>

class QLabel;

class SplashPage : public QWidget
{
    Q_OBJECT

public:
    explicit SplashPage(QWidget *parent = nullptr);

private:
    QLabel *m_logoLabel;
    QLabel *m_titleLabel;
    QLabel *m_subTitleLabel;
    QLabel *m_versionLabel;
};

#endif // SPLASHPAGE_H
