QT       += core gui widgets multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    albumpage.cpp \
    beeptestpage.cpp \
    bottombar.cpp \
    browserpage.cpp \
    camerapage.cpp \
    filepage.cpp \
    homepage.cpp \
    keyboardwidget.cpp \
    ledtestpage.cpp \
    main.cpp \
    mainwindow.cpp \
    musicpage.cpp \
    notepadpage.cpp \
    sensortestpage.cpp \
    settingpage.cpp \
    splashpage.cpp \
    systeminfopage.cpp \
    titlebar.cpp \
    videopage.cpp \
    wavepage.cpp \
    wavewidget.cpp

HEADERS += \
    albumpage.h \
    beeptestpage.h \
    bottombar.h \
    browserpage.h \
    camerapage.h \
    filepage.h \
    homepage.h \
    keyboardwidget.h \
    ledtestpage.h \
    mainwindow.h \
    musicpage.h \
    notepadpage.h \
    sensortestpage.h \
    settingpage.h \
    splashpage.h \
    systeminfopage.h \
    titlebar.h \
    videopage.h \
    wavepage.h \
    wavewidget.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
