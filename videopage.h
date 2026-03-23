#ifndef VIDEOPAGE_H
#define VIDEOPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;
class QSlider;
class QMediaPlayer;
class QVideoWidget;
class QMouseEvent;
class QKeyEvent;

class VideoPage : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPage(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void openFile();
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void toggleFullScreen();
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void setPosition(int position);
    void setVolume(int volume);
    void exitFullScreen();

private:
    QString formatTime(qint64 ms);
    void updateExitButtonPosition();

private:
    QMediaPlayer *m_player;
    QVideoWidget *m_videoWidget;

    QLabel *m_titleLabel;
    QLabel *m_fileLabel;
    QLabel *m_timeLabel;
    QLabel *m_statusLabel;

    QSlider *m_progressSlider;
    QSlider *m_volumeSlider;

    QPushButton *m_openBtn;
    QPushButton *m_playBtn;
    QPushButton *m_pauseBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_fullScreenBtn;

    QPushButton *m_exitFullScreenBtn;

    qint64 m_duration;
    bool m_isFullScreen;
};

#endif // VIDEOPAGE_H
