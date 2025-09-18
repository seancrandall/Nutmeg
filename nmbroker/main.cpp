#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

#include "dbaccess/nmdatabase.h"
#include "settings.h"
#include "websocketserver.h"

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

    // Determine WebSocket port: env var overrides settings, else default 8787
    quint16 wsPort = static_cast<quint16>(
        qvariant_cast<uint>(s.value("websocket/port", 8787))
    );
    const QByteArray envPort = qgetenv("NUTMEG_WS_PORT");
    if (!envPort.isEmpty()) {
        bool ok = false;
        const int p = QString::fromUtf8(envPort).toInt(&ok);
        if (ok && p > 0 && p <= 65535) {
            wsPort = static_cast<quint16>(p);
        }
    }

    Nutmeg::WebSocketServer wsServer(wsPort);

    return a.exec();
}
