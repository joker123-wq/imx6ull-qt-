#ifndef ALBUMPAGE_H
#define ALBUMPAGE_H

#include <QWidget>

class QListWidget;
class QLabel;
class QString;

class AlbumPage : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumPage(QWidget *parent = nullptr);

public slots:
    void loadImages();

private slots:
    void chooseFolder();
    void showImage();
    void deleteImage();

private:
    QString m_currentDir;
    QListWidget *m_listWidget;
    QLabel *m_previewLabel;
    QLabel *m_statusLabel;
};

#endif // ALBUMPAGE_H
