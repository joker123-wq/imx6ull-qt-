#include "videopage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QFileDialog>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QUrl>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>

VideoPage::VideoPage(QWidget *parent)
    : QWidget(parent), m_duration(0), m_isFullScreen(false)
{
    setStyleSheet("background-color: #F4F7FA;");

    m_player = new QMediaPlayer(this);
    m_videoWidget = new QVideoWidget(this);
    m_videoWidget->setStyleSheet("background:black;");

    m_titleLabel = new QLabel("视频播放器", this);
    m_titleLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    m_fileLabel = new QLabel("未选择视频文件", this);
    m_fileLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    m_timeLabel = new QLabel("00:00 / 00:00", this);
    m_timeLabel->setStyleSheet("font-size: 16px; color: #22313F;");

    m_statusLabel = new QLabel("状态：请选择视频文件", this);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    m_progressSlider = new QSlider(Qt::Horizontal, this);
    m_progressSlider->setRange(0, 0);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(80);

    m_openBtn = new QPushButton("选择视频", this);
    m_playBtn = new QPushButton("播放", this);
    m_pauseBtn = new QPushButton("暂停", this);
    m_stopBtn = new QPushButton("停止", this);
    m_fullScreenBtn = new QPushButton("全屏", this);

    QString btnStyle =
        "QPushButton { background:#4C8DFF; color:white; border-radius:10px; font-size:18px; font-weight:bold; min-height:42px; }"
        "QPushButton:hover { background:#3C7CE8; }"
        "QPushButton:pressed { background:#2F6BD0; }";

    m_openBtn->setStyleSheet(btnStyle);
    m_playBtn->setStyleSheet(btnStyle);
    m_pauseBtn->setStyleSheet(btnStyle);
    m_stopBtn->setStyleSheet(btnStyle);
    m_fullScreenBtn->setStyleSheet(btnStyle);

    m_player->setVideoOutput(m_videoWidget);
    m_player->setVolume(80);

    // 全屏退出按钮：挂在视频窗口上
    m_exitFullScreenBtn = new QPushButton("✕", m_videoWidget);
    m_exitFullScreenBtn->setFixedSize(56, 56);
    m_exitFullScreenBtn->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(0, 0, 0, 140);"
        "color: white;"
        "border: none;"
        "border-radius: 28px;"
        "font-size: 26px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(0, 0, 0, 190);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(0, 0, 0, 230);"
        "}"
    );
    m_exitFullScreenBtn->hide();

    connect(m_openBtn, &QPushButton::clicked, this, &VideoPage::openFile);
    connect(m_playBtn, &QPushButton::clicked, this, &VideoPage::playVideo);
    connect(m_pauseBtn, &QPushButton::clicked, this, &VideoPage::pauseVideo);
    connect(m_stopBtn, &QPushButton::clicked, this, &VideoPage::stopVideo);
    connect(m_fullScreenBtn, &QPushButton::clicked, this, &VideoPage::toggleFullScreen);
    connect(m_exitFullScreenBtn, &QPushButton::clicked, this, &VideoPage::exitFullScreen);

    connect(m_player, &QMediaPlayer::positionChanged, this, &VideoPage::updatePosition);
    connect(m_player, &QMediaPlayer::durationChanged, this, &VideoPage::updateDuration);

    connect(m_progressSlider, &QSlider::sliderMoved, this, &VideoPage::setPosition);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &VideoPage::setVolume);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_openBtn);
    controlLayout->addWidget(m_playBtn);
    controlLayout->addWidget(m_pauseBtn);
    controlLayout->addWidget(m_stopBtn);
    controlLayout->addWidget(m_fullScreenBtn);

    QHBoxLayout *volumeLayout = new QHBoxLayout;
    volumeLayout->addWidget(new QLabel("音量：", this));
    volumeLayout->addWidget(m_volumeSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 12, 20, 12);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_fileLabel);
    mainLayout->addWidget(m_videoWidget, 1);
    mainLayout->addWidget(m_progressSlider);
    mainLayout->addWidget(m_timeLabel);
    mainLayout->addLayout(volumeLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(m_statusLabel);

    // 监听视频窗口事件：双击切换全屏、尺寸变化时更新退出按钮位置
    m_videoWidget->installEventFilter(this);
}

void VideoPage::openFile()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        "选择视频文件",
        "",
        "Video Files (*.mp4 *.avi *.mkv *.mov *.wmv)"
    );

    if (file.isEmpty())
        return;

    m_player->setMedia(QUrl::fromLocalFile(file));
    m_fileLabel->setText("当前文件：" + QFileInfo(file).fileName());
    m_statusLabel->setText("状态：文件已加载");
}

void VideoPage::playVideo()
{
    if (m_player->media().isNull())
    {
        m_statusLabel->setText("状态：请先选择视频文件");
        return;
    }

    m_player->play();
    m_statusLabel->setText("状态：正在播放");
}

void VideoPage::pauseVideo()
{
    m_player->pause();
    m_statusLabel->setText("状态：已暂停");
}

void VideoPage::stopVideo()
{
    m_player->stop();
    m_progressSlider->setValue(0);
    m_statusLabel->setText("状态：已停止");
}

void VideoPage::toggleFullScreen()
{
    m_isFullScreen = !m_isFullScreen;
    m_videoWidget->setFullScreen(m_isFullScreen);

    if (m_isFullScreen)
    {
        updateExitButtonPosition();
        m_exitFullScreenBtn->show();
        m_exitFullScreenBtn->raise();
        m_fullScreenBtn->setText("退出全屏");
        m_statusLabel->setText("状态：全屏播放中");
    }
    else
    {
        m_exitFullScreenBtn->hide();
        m_fullScreenBtn->setText("全屏");
        m_statusLabel->setText("状态：已退出全屏");
    }
}

void VideoPage::exitFullScreen()
{
    if (!m_isFullScreen)
        return;

    m_isFullScreen = false;
    m_videoWidget->setFullScreen(false);
    m_exitFullScreenBtn->hide();
    m_fullScreenBtn->setText("全屏");
    m_statusLabel->setText("状态：已退出全屏");
}

void VideoPage::updatePosition(qint64 position)
{
    if (!m_progressSlider->isSliderDown())
        m_progressSlider->setValue(static_cast<int>(position));

    m_timeLabel->setText(formatTime(position) + " / " + formatTime(m_duration));
}

void VideoPage::updateDuration(qint64 duration)
{
    m_duration = duration;
    m_progressSlider->setRange(0, static_cast<int>(duration));
    m_timeLabel->setText("00:00 / " + formatTime(duration));
}

void VideoPage::setPosition(int position)
{
    m_player->setPosition(position);
}

void VideoPage::setVolume(int volume)
{
    m_player->setVolume(volume);
}

QString VideoPage::formatTime(qint64 ms)
{
    int secs = static_cast<int>(ms / 1000);
    int mins = secs / 60;
    secs %= 60;

    return QString("%1:%2")
            .arg(mins, 2, 10, QLatin1Char('0'))
            .arg(secs, 2, 10, QLatin1Char('0'));
}

void VideoPage::updateExitButtonPosition()
{
    if (!m_exitFullScreenBtn || !m_videoWidget)
        return;

    int x = m_videoWidget->width() - m_exitFullScreenBtn->width() - 16;
    int y = 16;
    m_exitFullScreenBtn->move(x, y);
}

void VideoPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && m_isFullScreen)
    {
        exitFullScreen();
        return;
    }

    QWidget::keyPressEvent(event);
}

bool VideoPage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_videoWidget)
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            toggleFullScreen();
            return true;
        }
        else if (event->type() == QEvent::Resize)
        {
            updateExitButtonPosition();
        }
    }

    return QWidget::eventFilter(watched, event);
}
