#include "filepage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

FilePage::FilePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F7FA;");

    QLabel *title = new QLabel("文件管理", this);
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #22313F;");

    m_pathLabel = new QLabel(this);
    m_pathLabel->setStyleSheet("font-size: 16px; color: #22313F;");

    m_statusLabel = new QLabel("状态：待加载", this);
    m_statusLabel->setStyleSheet("font-size: 16px; color: #5B6B79;");

    m_listWidget = new QListWidget(this);
    m_listWidget->setStyleSheet(
        "QListWidget {"
        "background: white;"
        "border: 2px solid #D5DEE7;"
        "border-radius: 12px;"
        "font-size: 18px;"
        "}"
    );

    m_upButton = new QPushButton("上一级", this);
    m_refreshButton = new QPushButton("刷新", this);

    m_upButton->setMinimumHeight(42);
    m_refreshButton->setMinimumHeight(42);

    connect(m_upButton, &QPushButton::clicked, this, &FilePage::goUp);
    connect(m_refreshButton, &QPushButton::clicked, this, &FilePage::refreshList);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *) {
        openCurrentItem();
    });

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_upButton);
    topLayout->addWidget(m_refreshButton);
    topLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 20, 28, 20);
    mainLayout->setSpacing(12);
    mainLayout->addWidget(title);
    mainLayout->addWidget(m_pathLabel);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_listWidget, 1);
    mainLayout->addWidget(m_statusLabel);

    loadPath(QDir::homePath());
}

void FilePage::goUp()
{
    QDir dir(m_currentPath);
    if (dir.cdUp())
        loadPath(dir.absolutePath());
}

void FilePage::refreshList()
{
    loadPath(m_currentPath);
}

void FilePage::openCurrentItem()
{
    QListWidgetItem *item = m_listWidget->currentItem();
    if (!item)
        return;

    QString fullPath = item->data(Qt::UserRole).toString();
    QFileInfo info(fullPath);

    if (info.isDir())
    {
        loadPath(fullPath);
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
    m_statusLabel->setText("状态：已打开文件 " + info.fileName());
}

QString FilePage::fileTypeText(const QString &filePath) const
{
    QFileInfo info(filePath);
    if (info.isDir())
        return "[目录] ";
    return "[文件] ";
}

void FilePage::loadPath(const QString &path)
{
    m_currentPath = path;
    m_listWidget->clear();
    m_pathLabel->setText("当前路径：" + m_currentPath);

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
                                           QDir::DirsFirst | QDir::Name);

    for (const QFileInfo &info : list)
    {
        QString text = fileTypeText(info.absoluteFilePath()) + info.fileName();
        QListWidgetItem *item = new QListWidgetItem(text, m_listWidget);
        item->setData(Qt::UserRole, info.absoluteFilePath());
    }

    m_statusLabel->setText(QString("状态：共 %1 项").arg(list.size()));
}
