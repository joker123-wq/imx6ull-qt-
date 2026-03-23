#include "camerapage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDir>
#include <QDateTime>
#include <QProcess>
#include <QPixmap>
#include <QTimer>
#include <QSizePolicy>

CameraPage::CameraPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: black;");

    m_previewLabel = new QLabel(this);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("background:black;");
    m_previewLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_statusLabel = new QLabel("状态：待预览", this);
    m_statusLabel->setStyleSheet(
        "QLabel {"
        "color:white;"
        "background:rgba(0,0,0,120);"
        "border-radius:8px;"
        "padding:6px 10px;"
        "font-size:16px;"
        "}"
    );
    m_statusLabel->setAlignment(Qt::AlignCenter);

    QPushButton *captureBtn = new QPushButton("拍照", this);
    captureBtn->setFixedHeight(50);
    captureBtn->setStyleSheet(
        "QPushButton {"
        "background:white;"
        "border-radius:12px;"
        "font-size:20px;"
        "font-weight:bold;"
        "padding:8px 20px;"
        "}"
    );

    QPushButton *albumBtn = new QPushButton("相册", this);
    albumBtn->setFixedHeight(50);
    albumBtn->setStyleSheet(
        "QPushButton {"
        "background:rgba(255,255,255,180);"
        "border-radius:12px;"
        "font-size:18px;"
        "font-weight:bold;"
        "padding:8px 20px;"
        "}"
    );

    connect(captureBtn, &QPushButton::clicked, this, &CameraPage::capturePhoto);
    connect(albumBtn, &QPushButton::clicked, this, &CameraPage::openAlbumRequested);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(albumBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(captureBtn);
    btnLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(8);
    mainLayout->addWidget(m_previewLabel, 1);
    mainLayout->addWidget(m_statusLabel, 0);
    mainLayout->addLayout(btnLayout);

    m_previewFile = "/tmp/camera_preview.jpg";

    m_previewTimer = new QTimer(this);
    connect(m_previewTimer, &QTimer::timeout, this, &CameraPage::updatePreview);

    // 先别启动太快，避免频繁打开摄像头
    m_previewTimer->start(1500);

    updatePreview();
}

void CameraPage::updatePreview()
{
#ifdef Q_OS_WIN
    m_statusLabel->setText("状态：Windows下不启用 /dev/video1 预览");
    m_previewLabel->setText("Windows 下无 /dev/video1 预览");
    return;
#else
    QString cmd = QString(
        "ffmpeg -f v4l2 "
        "-input_format yuyv422 "
        "-video_size 320x240 "
        "-i /dev/video1 "
        "-frames:v 1 -y %1 "
        ">/tmp/camera_preview.log 2>&1"
    ).arg(m_previewFile);

    int ret = QProcess::execute("sh", QStringList() << "-c" << cmd);

    if (ret != 0)
    {
        m_statusLabel->setText("状态：预览刷新失败");
        return;
    }

    QPixmap pix(m_previewFile);
    if (!pix.isNull())
    {
        m_previewLabel->setPixmap(
            pix.scaled(m_previewLabel->size(),
                       Qt::KeepAspectRatioByExpanding,
                       Qt::SmoothTransformation)
        );
        m_statusLabel->setText("状态：预览中");
    }
    else
    {
        m_statusLabel->setText("状态：预览图片加载失败");
    }
#endif
}

void CameraPage::capturePhoto()
{
#ifdef Q_OS_WIN
    m_statusLabel->setText("状态：Windows下不支持 /dev/video1 拍照");
    return;
#else
    QDir dir;
    if (!dir.exists("photos"))
        dir.mkdir("photos");

    QString filePath = QString("photos/photo_%1.jpg")
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    QString cmd = QString(
        "ffmpeg -f v4l2 "
        "-input_format yuyv422 "
        "-video_size 640x480 "
        "-i /dev/video1 "
        "-frames:v 1 -y %1 "
        ">/tmp/camera_capture.log 2>&1"
    ).arg(filePath);

    int ret = QProcess::execute("sh", QStringList() << "-c" << cmd);

    if (ret != 0)
    {
        m_statusLabel->setText("状态：拍照失败");
        return;
    }

    QPixmap pix(filePath);
    if (!pix.isNull())
    {
        m_previewLabel->setPixmap(
            pix.scaled(m_previewLabel->size(),
                       Qt::KeepAspectRatioByExpanding,
                       Qt::SmoothTransformation)
        );
    }

    m_statusLabel->setText("状态：拍照成功，已保存到 " + filePath);
    emit photoCaptured();
#endif
}
