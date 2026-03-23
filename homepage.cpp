#include "homepage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QTimer>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F5F7FB;");

    QLabel *titleLabel = new QLabel("SmartPad", this);
    titleLabel->setStyleSheet(
        "font-size: 34px;"
        "font-weight: bold;"
        "color: #1F2A36;"
    );

    QLabel *subTitleLabel = new QLabel("欢迎使用 i.MX6ULL 智能终端", this);
    subTitleLabel->setStyleSheet(
        "font-size: 17px;"
        "color: #7B8794;"
    );

    QWidget *appArea = new QWidget(this);
    appArea->setStyleSheet("background: transparent;");
    QGridLayout *grid = new QGridLayout(appArea);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setHorizontalSpacing(18);
    grid->setVerticalSpacing(18);

    grid->addWidget(createAppCard("🎵", "音乐", "本地音频", SIGNAL(openMusicRequested())), 0, 0);
    grid->addWidget(createAppCard("🎬", "视频", "本地视频", SIGNAL(openVideoRequested())), 0, 1);
    grid->addWidget(createAppCard("📁", "文件", "文件管理", SIGNAL(openFileRequested())), 0, 2);
    grid->addWidget(createAppCard("📷", "相机", "拍照预览", SIGNAL(openCameraRequested())), 0, 3);

    grid->addWidget(createAppCard("🖼", "相册", "图片浏览", SIGNAL(openAlbumRequested())), 1, 0);
    grid->addWidget(createAppCard("🌐", "浏览器", "网页浏览", SIGNAL(openBrowserRequested())), 1, 1);
    grid->addWidget(createAppCard("🧪", "传感器", "数值测试", SIGNAL(openSensorRequested())), 1, 2);
    grid->addWidget(createAppCard("📝", "记事本", "文本记录", SIGNAL(openNotepadRequested())), 1, 3);

    grid->addWidget(createAppCard("📈", "波形", "实时曲线", SIGNAL(openWaveRequested())), 2, 0);
    grid->addWidget(createAppCard("⚙", "设置", "系统配置", SIGNAL(openSettingRequested())), 2, 1);
    grid->addWidget(createAppCard("ℹ", "系统", "状态信息", SIGNAL(openSystemInfoRequested())), 2, 2);
    grid->addWidget(createAppCard("💡", "LED", "灯光控制", SIGNAL(openLedRequested())), 2, 3);

    grid->addWidget(createAppCard("🔔", "BEEP", "蜂鸣器", SIGNAL(openBeepRequested())), 3, 0);
    grid->addWidget(createAppCard("⏻", "退出", "关闭程序", SIGNAL(exitRequested())), 3, 1);

    QWidget *statusCard = createStatusCard();

    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(24);
    contentLayout->addWidget(appArea, 3);
    contentLayout->addWidget(statusCard, 2);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 24, 28, 20);
    mainLayout->setSpacing(18);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subTitleLabel);
    mainLayout->addLayout(contentLayout, 1);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        m_timeValueLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    });
    timer->start(1000);
    m_timeValueLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
}

QWidget *HomePage::createAppCard(const QString &emoji,
                                 const QString &title,
                                 const QString &subTitle,
                                 const char *signalName)
{
    QPushButton *btn = new QPushButton(this);
    btn->setFixedSize(150, 118);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(
        "QPushButton {"
        "background: white;"
        "border: none;"
        "border-radius: 26px;"
        "padding: 0px;"
        "}"
        "QPushButton:hover {"
        "background: #EEF4FB;"
        "}"
        "QPushButton:pressed {"
        "background: #E4ECF6;"
        "}"
    );

    QLabel *iconLabel = new QLabel(emoji, btn);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("font-size: 30px;");

    QLabel *titleLabel = new QLabel(title, btn);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 19px;"
        "font-weight: bold;"
        "color: #1F2A36;"
    );

    QLabel *subLabel = new QLabel(subTitle, btn);
    subLabel->setAlignment(Qt::AlignCenter);
    subLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #8A97A5;"
    );

    QVBoxLayout *layout = new QVBoxLayout(btn);
    layout->setContentsMargins(12, 14, 12, 14);
    layout->setSpacing(6);
    layout->addWidget(iconLabel);
    layout->addWidget(titleLabel);
    layout->addWidget(subLabel);

    connect(btn, SIGNAL(clicked()), this, signalName);

    return btn;
}

QWidget *HomePage::createStatusCard()
{
    QWidget *card = new QWidget(this);
    card->setMinimumWidth(280);
    card->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #4A90E2, stop:1 #357ABD);"
        "border-radius: 30px;"
        "}"
    );

    QLabel *smallTitle = new QLabel("系统状态", card);
    smallTitle->setStyleSheet(
        "font-size: 18px;"
        "color: rgba(255,255,255,0.9);"
        "font-weight: bold;"
    );

    m_timeTitleLabel = new QLabel("当前时间", card);
    m_timeTitleLabel->setStyleSheet(
        "font-size: 16px;"
        "color: rgba(255,255,255,0.85);"
    );

    m_timeValueLabel = new QLabel("00:00:00", card);
    m_timeValueLabel->setStyleSheet(
        "font-size: 42px;"
        "font-weight: bold;"
        "color: white;"
    );

    m_infoLabel = new QLabel("音视频播放\n相机拍照\n传感器采集\n文件管理", card);
    m_infoLabel->setStyleSheet(
        "font-size: 18px;"
        "color: rgba(255,255,255,0.95);"
        "line-height: 1.6;"
    );

    QLabel *footLabel = new QLabel("Smart Embedded Terminal", card);
    footLabel->setStyleSheet(
        "font-size: 14px;"
        "color: rgba(255,255,255,0.78);"
    );

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(26, 24, 26, 24);
    layout->setSpacing(12);
    layout->addWidget(smallTitle);
    layout->addSpacing(10);
    layout->addWidget(m_timeTitleLabel);
    layout->addWidget(m_timeValueLabel);
    layout->addSpacing(16);
    layout->addWidget(m_infoLabel);
    layout->addStretch();
    layout->addWidget(footLabel);

    return card;
}
