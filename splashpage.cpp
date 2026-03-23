#include "splashpage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QPixmap>

SplashPage::SplashPage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(1024, 600);
    setWindowFlags(Qt::FramelessWindowHint);

    // ===================== 背景图 =====================
    QLabel *bg = new QLabel(this);
    bg->setGeometry(0, 0, 1024, 600);
    bg->setPixmap(QPixmap(":/boot.jpg"));
    bg->setScaledContents(true);
    bg->lower();

    // ===================== 整体遮罩 =====================
    QWidget *overlay = new QWidget(this);
    overlay->setGeometry(0, 0, 1024, 600);
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 90);");
    overlay->lower();
    bg->lower();

    // ===================== 中间内容卡片 =====================
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setGeometry(212, 140, 600, 320);
    contentWidget->setStyleSheet(
        "background-color: rgba(0, 0, 0, 110);"
        "border-radius: 24px;"
    );

    // ===================== 文字控件 =====================
    m_logoLabel = new QLabel("SmartPad", contentWidget);
    m_logoLabel->setAlignment(Qt::AlignCenter);
    m_logoLabel->setStyleSheet(
        "font-size: 58px;"
        "font-weight: bold;"
        "color: white;"
        "background: transparent;"
    );

    m_titleLabel = new QLabel("i.MX6ULL 智能终端系统", contentWidget);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "color: rgba(255,255,255,0.95);"
        "background: transparent;"
    );

    m_subTitleLabel = new QLabel("音频 / 视频 / 相机 / 相册 / 文件 / 传感器 / 外设控制", contentWidget);
    m_subTitleLabel->setAlignment(Qt::AlignCenter);
    m_subTitleLabel->setWordWrap(true);
    m_subTitleLabel->setStyleSheet(
        "font-size: 18px;"
        "color: rgba(255,255,255,0.82);"
        "background: transparent;"
    );

    m_versionLabel = new QLabel("Version 1.0", contentWidget);
    m_versionLabel->setAlignment(Qt::AlignCenter);
    m_versionLabel->setStyleSheet(
        "font-size: 16px;"
        "color: rgba(255,255,255,0.60);"
        "background: transparent;"
    );

    // ===================== 布局 =====================
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(40, 35, 40, 35);
    layout->setSpacing(0);

    layout->addStretch();
    layout->addWidget(m_logoLabel);
    layout->addSpacing(18);
    layout->addWidget(m_titleLabel);
    layout->addSpacing(12);
    layout->addWidget(m_subTitleLabel);
    layout->addSpacing(26);
    layout->addWidget(m_versionLabel);
    layout->addStretch();
}
