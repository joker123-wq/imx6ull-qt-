#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class TitleBar;
class HomePage;
class BottomBar;
class QStackedWidget;

class MusicPage;
class VideoPage;
class FilePage;
class CameraPage;
class AlbumPage;
class BrowserPage;
class SensorTestPage;
class NotepadPage;
class SettingPage;
class SystemInfoPage;
class LedTestPage;
class BeepTestPage;
class WavePage;



class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void showHomePage();
    void showMusicPage();
    void showVideoPage();
    void showFilePage();
    void showCameraPage();
    void showAlbumPage();
    void showBrowserPage();
    void showSensorPage();
    void showNotepadPage();
    void showSettingPage();
    void showSystemInfoPage();
    void showLedPage();
    void showBeepPage();
    void handleExitRequested();
    void showWavePage();

private:
    TitleBar *m_titleBar;
    QStackedWidget *m_stackedWidget;
    HomePage *m_homePage;
    BottomBar *m_bottomBar;

    MusicPage *m_musicPage;
    VideoPage *m_videoPage;
    FilePage *m_filePage;
    CameraPage *m_cameraPage;
    AlbumPage *m_albumPage;
    BrowserPage *m_browserPage;
    SensorTestPage *m_sensorPage;
    NotepadPage *m_notepadPage;
    SettingPage *m_settingPage;
    SystemInfoPage *m_systemInfoPage;
    LedTestPage *m_ledPage;
    BeepTestPage *m_beepPage;
    WavePage *m_wavePage;
};

#endif // MAINWINDOW_H
