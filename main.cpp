#include <QApplication>
#include <QTimer>
#include "mainwindow.h"
#include "splashpage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SplashPage splash;
    MainWindow w;

    splash.show();

    QTimer::singleShot(2000, [&]() {
        splash.close();
        w.show();
    });

    return a.exec();
}
