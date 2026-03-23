#include "browserpage.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextBrowser>
#include <QUrl>
#include <QLabel>

BrowserPage::BrowserPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("浏览器", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    m_urlEdit = new QLineEdit(this);
    m_urlEdit->setPlaceholderText("输入网址，例如 https://www.qt.io");

    QPushButton *goBtn = new QPushButton("打开", this);
    QPushButton *preset1 = new QPushButton("Qt官网", this);
    QPushButton *preset2 = new QPushButton("本地帮助", this);

    m_browser = new QTextBrowser(this);
    m_browser->setOpenExternalLinks(true);
    m_browser->setHtml("<h3>轻量浏览器页面</h3><p>这个版本先用 QTextBrowser 实现轻量网页/HTML 浏览。</p>");

    connect(goBtn, &QPushButton::clicked, this, &BrowserPage::openUrl);
    connect(preset1, &QPushButton::clicked, this, &BrowserPage::openPreset1);
    connect(preset2, &QPushButton::clicked, this, &BrowserPage::openPreset2);

    QHBoxLayout *top = new QHBoxLayout;
    top->addWidget(m_urlEdit, 1);
    top->addWidget(goBtn);
    top->addWidget(preset1);
    top->addWidget(preset2);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(12);
    layout->addWidget(title);
    layout->addLayout(top);
    layout->addWidget(m_browser, 1);
}

void BrowserPage::openUrl()
{
    QUrl url = QUrl::fromUserInput(m_urlEdit->text());
    if (url.isValid())
        m_browser->setSource(url);
}

void BrowserPage::openPreset1()
{
    m_urlEdit->setText("https://www.qt.io");
    openUrl();
}

void BrowserPage::openPreset2()
{
    m_browser->setHtml("<h2>本地帮助</h2><p>这里可以放你的项目说明、使用手册、版本信息。</p>");
}

