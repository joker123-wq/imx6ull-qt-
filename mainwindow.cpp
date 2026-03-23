#include "mainwindow.h"
#include "titlebar.h"
#include "homepage.h"
#include "bottombar.h"

#include "musicpage.h"
#include "videopage.h"
#include "filepage.h"
#include "camerapage.h"
#include "albumpage.h"
#include "browserpage.h"
#include "sensortestpage.h"
#include "notepadpage.h"
#include "settingpage.h"
#include "systeminfopage.h"
#include "ledtestpage.h"
#include "beeptestpage.h"
#include "wavepage.h"

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(1024, 600);
    setMinimumSize(1024, 600);
    setMaximumSize(1024, 600);
    setWindowTitle("SmartPad");
    setStyleSheet("background-color: #EAEFF3;");

    m_titleBar = new TitleBar(this);
    m_stackedWidget = new QStackedWidget(this);
    m_homePage = new HomePage(this);
    m_bottomBar = new BottomBar(this);

    m_musicPage = new MusicPage(this);
    m_videoPage = new VideoPage(this);
    m_filePage = new FilePage(this);
    m_cameraPage = new CameraPage(this);
    m_albumPage = new AlbumPage(this);
    m_browserPage = new BrowserPage(this);
    m_sensorPage = new SensorTestPage(this);
    m_notepadPage = new NotepadPage(this);
    m_settingPage = new SettingPage(this);
    m_systemInfoPage = new SystemInfoPage(this);
    m_ledPage = new LedTestPage(this);
    m_beepPage = new BeepTestPage(this);
    m_wavePage = new WavePage(this);

    m_stackedWidget->addWidget(m_homePage);
    m_stackedWidget->addWidget(m_musicPage);
    m_stackedWidget->addWidget(m_videoPage);
    m_stackedWidget->addWidget(m_filePage);
    m_stackedWidget->addWidget(m_cameraPage);
    m_stackedWidget->addWidget(m_albumPage);
    m_stackedWidget->addWidget(m_browserPage);
    m_stackedWidget->addWidget(m_sensorPage);
    m_stackedWidget->addWidget(m_notepadPage);
    m_stackedWidget->addWidget(m_settingPage);
    m_stackedWidget->addWidget(m_systemInfoPage);
    m_stackedWidget->addWidget(m_ledPage);
    m_stackedWidget->addWidget(m_beepPage);
    m_stackedWidget->addWidget(m_wavePage);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_titleBar);
    mainLayout->addWidget(m_stackedWidget,1);
    mainLayout->addWidget(m_bottomBar);

    connect(m_homePage, &HomePage::openMusicRequested, this, &MainWindow::showMusicPage);
    connect(m_homePage, &HomePage::openVideoRequested, this, &MainWindow::showVideoPage);
    connect(m_homePage, &HomePage::openFileRequested, this, &MainWindow::showFilePage);
    connect(m_homePage, &HomePage::openCameraRequested, this, &MainWindow::showCameraPage);
    connect(m_homePage, &HomePage::openAlbumRequested, this, &MainWindow::showAlbumPage);
    connect(m_homePage, &HomePage::openBrowserRequested, this, &MainWindow::showBrowserPage);
    connect(m_homePage, &HomePage::openSensorRequested, this, &MainWindow::showSensorPage);
    connect(m_homePage, &HomePage::openNotepadRequested, this, &MainWindow::showNotepadPage);
    connect(m_homePage, &HomePage::openSettingRequested, this, &MainWindow::showSettingPage);
    connect(m_homePage, &HomePage::openSystemInfoRequested, this, &MainWindow::showSystemInfoPage);
    connect(m_homePage, &HomePage::openLedRequested, this, &MainWindow::showLedPage);
    connect(m_homePage, &HomePage::openBeepRequested, this, &MainWindow::showBeepPage);
    connect(m_homePage, &HomePage::exitRequested, this, &MainWindow::handleExitRequested);
    connect(m_homePage, &HomePage::openWaveRequested, this, &MainWindow::showWavePage);

    connect(m_bottomBar, &BottomBar::homeClicked, this, &MainWindow::showHomePage);
    connect(m_bottomBar, &BottomBar::backClicked, this, &MainWindow::showHomePage);

    connect(m_cameraPage, &CameraPage::photoCaptured,
            m_albumPage, &AlbumPage::loadImages);

    connect(m_cameraPage, &CameraPage::openAlbumRequested,
            this, &MainWindow::showAlbumPage);
    connect(m_settingPage, &SettingPage::volumeChanged,
            m_titleBar, &TitleBar::setVolumeValue);

    connect(m_settingPage, &SettingPage::brightnessChanged,
            m_titleBar, &TitleBar::setBrightnessValue);

    showHomePage();
}

void MainWindow::showHomePage(){ m_stackedWidget->setCurrentWidget(m_homePage); }
void MainWindow::showMusicPage(){ m_stackedWidget->setCurrentWidget(m_musicPage); }
void MainWindow::showVideoPage(){ m_stackedWidget->setCurrentWidget(m_videoPage); }
void MainWindow::showFilePage(){ m_stackedWidget->setCurrentWidget(m_filePage); }
void MainWindow::showCameraPage(){ m_stackedWidget->setCurrentWidget(m_cameraPage); }
void MainWindow::showAlbumPage(){ m_stackedWidget->setCurrentWidget(m_albumPage); }
void MainWindow::showBrowserPage(){ m_stackedWidget->setCurrentWidget(m_browserPage); }
void MainWindow::showSensorPage(){ m_stackedWidget->setCurrentWidget(m_sensorPage); }
void MainWindow::showNotepadPage(){ m_stackedWidget->setCurrentWidget(m_notepadPage); }
void MainWindow::showSettingPage(){ m_stackedWidget->setCurrentWidget(m_settingPage); }
void MainWindow::showSystemInfoPage(){ m_stackedWidget->setCurrentWidget(m_systemInfoPage); }
void MainWindow::showLedPage(){ m_stackedWidget->setCurrentWidget(m_ledPage); }
void MainWindow::showBeepPage(){ m_stackedWidget->setCurrentWidget(m_beepPage); }
void MainWindow::showWavePage()
{
    m_stackedWidget->setCurrentWidget(m_wavePage);
}

void MainWindow::handleExitRequested()
{
    QApplication::quit();
}
