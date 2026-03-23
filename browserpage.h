#ifndef BROWSERPAGE_H
#define BROWSERPAGE_H

#include <QWidget>

class QLineEdit;
class QTextBrowser;

class BrowserPage : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserPage(QWidget *parent = nullptr);

private slots:
    void openUrl();
    void openPreset1();
    void openPreset2();

private:
    QLineEdit *m_urlEdit;
    QTextBrowser *m_browser;
};

#endif // BROWSERPAGE_H
