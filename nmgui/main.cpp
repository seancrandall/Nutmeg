#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QPixmap>
#include <QSplashScreen>
#include <QStyleFactory>

#include "logger.h"
#include "settings.h"
#include "cache/cache.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set the application to use the native style of the OS
    //QApplication::setStyle(QStyleFactory::create("Fusion")); // For a more generic approach, you might want to use:
    QApplication::setStyle(QStyleFactory::create(QStyleFactory::keys().contains("windows") ? "windows" :
                                                  QStyleFactory::keys().contains("macintosh") ? "macintosh" :
                                                  "Fusion"));

    // Initialize the object cache
    Nutmeg::initCaches();

    // Logo is included in the resource file
    QPixmap nutmegSplashScreen(":images/Logo.png");
    QSplashScreen splash(nutmegSplashScreen);
    splash.show();

    // Initialize the log file and setup the maximum and "preferred" number of lines
    // Once log grows beyond max number of lines, it is trimmed back to "preferred" size.
    Nutmeg::Logger::InitializeLogCount();
    Nutmeg::Settings settings;
    Nutmeg::Logger::maxLogLines = settings.maxLogLines;
    Nutmeg::Logger::preferredLogLines = settings.preferredLogLines;

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "nmgui_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }
    Nutmeg::MainWindow w;

    QIcon icon(":images/nutmeg.ico");
    w.setWindowIcon(icon);

    w.show();
    splash.finish(&w);
    return a.exec();
}
