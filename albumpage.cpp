#include "albumpage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QDir>
#include <QFileInfoList>
#include <QPixmap>
#include <QMessageBox>
#include <QFile>
#include <QSizePolicy>
#include <QFileDialog>

AlbumPage::AlbumPage(QWidget *parent)
    : QWidget(parent), m_currentDir("")
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("相册", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    QPushButton *chooseBtn = new QPushButton("选择目录", this);
    QPushButton *refreshBtn = new QPushButton("刷新图片", this);
    QPushButton *deleteBtn = new QPushButton("删除图片", this);

    chooseBtn->setMinimumHeight(42);
    refreshBtn->setMinimumHeight(42);
    deleteBtn->setMinimumHeight(42);

    QString blueBtn =
        "QPushButton { background:#4C8DFF; color:white; border-radius:10px; font-size:18px; font-weight:bold; }";
    QString redBtn =
        "QPushButton { background:#FF6B6B; color:white; border-radius:10px; font-size:18px; font-weight:bold; }";

    chooseBtn->setStyleSheet(blueBtn);
    refreshBtn->setStyleSheet(blueBtn);
    deleteBtn->setStyleSheet(redBtn);

    m_listWidget = new QListWidget(this);
    m_listWidget->setStyleSheet(
        "QListWidget {"
        "background: white;"
        "border: 2px solid #D5DEE7;"
        "border-radius: 12px;"
        "font-size: 16px;"
        "}"
    );

    m_previewLabel = new QLabel("预览区", this);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet(
        "background:white;"
        "border:2px solid #D5DEE7;"
        "border-radius:12px;"
        "font-size:18px;"
        "color:#7F8C8D;"
    );
    m_previewLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_statusLabel = new QLabel("状态：请选择图片目录", this);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    connect(chooseBtn, &QPushButton::clicked, this, &AlbumPage::chooseFolder);
    connect(refreshBtn, &QPushButton::clicked, this, &AlbumPage::loadImages);
    connect(deleteBtn, &QPushButton::clicked, this, &AlbumPage::deleteImage);
    connect(m_listWidget, &QListWidget::currentRowChanged, this, [=](int){
        showImage();
    });

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(chooseBtn);
    btnLayout->addWidget(refreshBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addStretch();

    QHBoxLayout *content = new QHBoxLayout;
    content->addWidget(m_listWidget, 1);
    content->addWidget(m_previewLabel, 2);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(12);
    layout->addWidget(title);
    layout->addLayout(btnLayout);
    layout->addLayout(content, 1);
    layout->addWidget(m_statusLabel);
}

void AlbumPage::chooseFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择图片目录");
    if (dir.isEmpty())
        return;

    m_currentDir = dir;
    loadImages();
}

void AlbumPage::loadImages()
{
    m_listWidget->clear();

    if (m_currentDir.isEmpty())
    {
        m_statusLabel->setText("状态：请先选择图片目录");
        m_previewLabel->clear();
        m_previewLabel->setText("预览区");
        return;
    }

    QDir dir(m_currentDir);
    if (!dir.exists())
    {
        m_statusLabel->setText("目录不存在：" + m_currentDir);
        m_previewLabel->clear();
        m_previewLabel->setText("预览区");
        return;
    }

    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";

    QFileInfoList list = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot, QDir::Time);

    for (int i = 0; i < list.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(list.at(i).fileName());
        item->setData(Qt::UserRole, list.at(i).absoluteFilePath());
        m_listWidget->addItem(item);
    }

    m_statusLabel->setText(QString("当前目录：%1，共 %2 张图片").arg(m_currentDir).arg(list.size()));

    if (list.size() > 0)
        m_listWidget->setCurrentRow(0);
    else
    {
        m_previewLabel->clear();
        m_previewLabel->setText("预览区");
    }
}

void AlbumPage::showImage()
{
    QListWidgetItem *item = m_listWidget->currentItem();
    if (!item)
        return;

    QString filePath = item->data(Qt::UserRole).toString();

    QPixmap pix(filePath);
    if (pix.isNull())
    {
        m_previewLabel->clear();
        m_previewLabel->setText("图片加载失败");
        return;
    }

    m_previewLabel->setPixmap(
        pix.scaled(m_previewLabel->size(),
                   Qt::KeepAspectRatioByExpanding,
                   Qt::SmoothTransformation)
    );
}

void AlbumPage::deleteImage()
{
    QListWidgetItem *item = m_listWidget->currentItem();
    if (!item)
    {
        m_statusLabel->setText("请先选择一张图片");
        return;
    }

    QString filePath = item->data(Qt::UserRole).toString();

    int ret = QMessageBox::question(
        this,
        "删除确认",
        "确定删除这张图片吗？",
        QMessageBox::Yes | QMessageBox::No
    );

    if (ret != QMessageBox::Yes)
        return;

    if (QFile::remove(filePath))
    {
        m_statusLabel->setText("删除成功");
        loadImages();
    }
    else
    {
        m_statusLabel->setText("删除失败（权限或文件不存在）");
    }
}
