#ifndef CAMERAPAGE_H
#define CAMERAPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;
class QTimer;

class CameraPage : public QWidget
{
    Q_OBJECT

public:
    explicit CameraPage(QWidget *parent = nullptr);

signals:
    void photoCaptured();
    void openAlbumRequested();

private slots:
    void updatePreview();
    void capturePhoto();

private:
    QLabel *m_previewLabel;
    QLabel *m_statusLabel;
    QTimer *m_previewTimer;
    QString m_previewFile;
};

#endif // CAMERAPAGE_H
