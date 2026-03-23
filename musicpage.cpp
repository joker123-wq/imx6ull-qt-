#include "musicpage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QFileDialog>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QUrl>

MusicPage::MusicPage(QWidget *parent)
    : QWidget(parent), m_duration(0)
{
    setStyleSheet("background-color: #F4F7FA;");

    m_player = new QMediaPlayer(this);

    m_titleLabel = new QLabel("音乐播放器", this);
    m_titleLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    m_fileLabel = new QLabel("未选择音频文件", this);
    m_fileLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    m_timeLabel = new QLabel("00:00 / 00:00", this);
    m_timeLabel->setStyleSheet("font-size: 16px; color: #22313F;");

    m_statusLabel = new QLabel("状态：请选择音频文件", this);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    m_progressSlider = new QSlider(Qt::Horizontal, this);
    m_progressSlider->setRange(0, 0);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(80);

    m_openBtn = new QPushButton("选择音频", this);
    m_playBtn = new QPushButton("播放", this);
    m_pauseBtn = new QPushButton("暂停", this);
    m_stopBtn = new QPushButton("停止", this);

    QString btnStyle =
        "QPushButton { background:#4C8DFF; color:white; border-radius:10px; font-size:18px; font-weight:bold; min-height:42px; }"
        "QPushButton:hover { background:#3C7CE8; }"
        "QPushButton:pressed { background:#2F6BD0; }";

    m_openBtn->setStyleSheet(btnStyle);
    m_playBtn->setStyleSheet(btnStyle);
    m_pauseBtn->setStyleSheet(btnStyle);
    m_stopBtn->setStyleSheet(btnStyle);

    connect(m_openBtn, &QPushButton::clicked, this, &MusicPage::openFile);
    connect(m_playBtn, &QPushButton::clicked, this, &MusicPage::playMusic);
    connect(m_pauseBtn, &QPushButton::clicked, this, &MusicPage::pauseMusic);
    connect(m_stopBtn, &QPushButton::clicked, this, &MusicPage::stopMusic);

    connect(m_player, &QMediaPlayer::positionChanged, this, &MusicPage::updatePosition);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MusicPage::updateDuration);

    connect(m_progressSlider, &QSlider::sliderMoved, this, &MusicPage::setPosition);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &MusicPage::setVolume);

    m_player->setVolume(80);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_openBtn);
    controlLayout->addWidget(m_playBtn);
    controlLayout->addWidget(m_pauseBtn);
    controlLayout->addWidget(m_stopBtn);

    QHBoxLayout *volumeLayout = new QHBoxLayout;
    volumeLayout->addWidget(new QLabel("音量：", this));
    volumeLayout->addWidget(m_volumeSlider);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 20, 28, 20);
    mainLayout->setSpacing(14);
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_fileLabel);
    mainLayout->addWidget(m_progressSlider);
    mainLayout->addWidget(m_timeLabel);
    mainLayout->addLayout(volumeLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addStretch();
}

void MusicPage::openFile()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        "选择音频文件",
        "",
        "Audio Files (*.mp3 *.wav *.aac *.ogg *.flac)"
    );

    if (file.isEmpty())
        return;

    m_player->setMedia(QUrl::fromLocalFile(file));
    m_fileLabel->setText("当前文件：" + QFileInfo(file).fileName());
    m_statusLabel->setText("状态：文件已加载");
}

void MusicPage::playMusic()
{
    if (m_player->media().isNull())
    {
        m_statusLabel->setText("状态：请先选择音频文件");
        return;
    }

    m_player->play();
    m_statusLabel->setText("状态：正在播放");
}

void MusicPage::pauseMusic()
{
    m_player->pause();
    m_statusLabel->setText("状态：已暂停");
}

void MusicPage::stopMusic()
{
    m_player->stop();
    m_progressSlider->setValue(0);
    m_statusLabel->setText("状态：已停止");
}

void MusicPage::updatePosition(qint64 position)
{
    if (!m_progressSlider->isSliderDown())
        m_progressSlider->setValue(static_cast<int>(position));

    m_timeLabel->setText(formatTime(position) + " / " + formatTime(m_duration));
}

void MusicPage::updateDuration(qint64 duration)
{
    m_duration = duration;
    m_progressSlider->setRange(0, static_cast<int>(duration));
    m_timeLabel->setText("00:00 / " + formatTime(duration));
}

void MusicPage::setPosition(int position)
{
    m_player->setPosition(position);
}

void MusicPage::setVolume(int volume)
{
    m_player->setVolume(volume);
}

QString MusicPage::formatTime(qint64 ms)
{
    int secs = static_cast<int>(ms / 1000);
    int mins = secs / 60;
    secs %= 60;

    return QString("%1:%2")
            .arg(mins, 2, 10, QLatin1Char('0'))
            .arg(secs, 2, 10, QLatin1Char('0'));
}
