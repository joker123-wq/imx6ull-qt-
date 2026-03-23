#ifndef MUSICPAGE_H
#define MUSICPAGE_H

#include <QWidget>

class QMediaPlayer;
class QPushButton;
class QLabel;
class QSlider;

class MusicPage : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPage(QWidget *parent = nullptr);

private slots:
    void openFile();
    void playMusic();
    void pauseMusic();
    void stopMusic();
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void setPosition(int position);
    void setVolume(int volume);

private:
    QString formatTime(qint64 ms);

private:
    QMediaPlayer *m_player;
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

    qint64 m_duration;
};

#endif // MUSICPAGE_H
