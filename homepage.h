#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

class QGridLayout;
class QLabel;

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);

signals:
    void openMusicRequested();
    void openVideoRequested();
    void openFileRequested();
    void openCameraRequested();
    void openAlbumRequested();
    void openBrowserRequested();
    void openSensorRequested();
    void openNotepadRequested();
    void openSettingRequested();
    void openSystemInfoRequested();
    void openLedRequested();
    void openBeepRequested();
    void openWaveRequested();
    void exitRequested();

private:
    QWidget *createAppCard(const QString &emoji,
                           const QString &title,
                           const QString &subTitle,
                           const char *signalName);

    QWidget *createStatusCard();

private:
    QLabel *m_timeTitleLabel;
    QLabel *m_timeValueLabel;
    QLabel *m_infoLabel;
};
#endif // HOMEPAGE_H
