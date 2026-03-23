#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QWidget>

class QLabel;
class QListWidget;
class QPushButton;

class FilePage : public QWidget
{
    Q_OBJECT

public:
    explicit FilePage(QWidget *parent = nullptr);

private slots:
    void goUp();
    void refreshList();
    void openCurrentItem();

private:
    void loadPath(const QString &path);
    QString fileTypeText(const QString &filePath) const;

private:
    QString m_currentPath;
    QLabel *m_pathLabel;
    QLabel *m_statusLabel;
    QListWidget *m_listWidget;
    QPushButton *m_upButton;
    QPushButton *m_refreshButton;
};

#endif // FILEPAGE_H
