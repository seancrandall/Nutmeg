#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

#include "dbaccess/nmdatabase.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "dmbroker_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    Nutmeg::Settings s;
    Nutmeg::NMDatabase *dm = new Nutmeg::NMDatabase();
    dm->server = s.server;
    dm->databasename = s.databaseName;
    dm->port = s.port;
    dm->username = s.username;
    dm->password = s.password;
    dm->connect();

    // dm->TestBed();
    // Nutmeg::TableModel model(*dm);

    return a.exec();
}
