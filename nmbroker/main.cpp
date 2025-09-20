#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QProcess>
#include <QDir>
#include <QHostAddress>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <memory>

#include "settings.h"
#include "websocketserver.h"
#include "dbaccess/databaseconnection.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("nmbroker"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.1"));

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

    // Command line options
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Nutmeg backend broker (WebSocket server)"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption portOpt({QStringLiteral("p"), QStringLiteral("port")},
                               QStringLiteral("Port to listen on (overrides env/settings)."),
                               QStringLiteral("port"));
    QCommandLineOption detachOpt({QStringLiteral("d"), QStringLiteral("detach")},
                                 QStringLiteral("Run in background (detach)."));
    QCommandLineOption bindOpt(QStringList{QStringLiteral("b"), QStringLiteral("bind")},
                               QStringLiteral("Bind address (IPv4/IPv6 literal or 'localhost')."),
                               QStringLiteral("addr"));
    // Database options
    QCommandLineOption dbHostOpt(QStringList{QStringLiteral("db-host")},
                                 QStringLiteral("Database host (overrides env/settings)."),
                                 QStringLiteral("host"));
    QCommandLineOption dbPortOpt(QStringList{QStringLiteral("db-port")},
                                 QStringLiteral("Database port (overrides env/settings)."),
                                 QStringLiteral("port"));
    QCommandLineOption dbUserOpt(QStringList{QStringLiteral("db-user")},
                                 QStringLiteral("Database user (overrides env/settings)."),
                                 QStringLiteral("user"));
    QCommandLineOption dbPassOpt(QStringList{QStringLiteral("db-password")},
                                 QStringLiteral("Database password (overrides env/settings)."),
                                 QStringLiteral("password"));
    QCommandLineOption dbNameOpt(QStringList{QStringLiteral("db-name")},
                                 QStringLiteral("Database schema/name (overrides env/settings)."),
                                 QStringLiteral("name"));
    QCommandLineOption pidFileOpt(QStringList{QStringLiteral("pidfile")},
                                  QStringLiteral("Write the process ID to this file after start."),
                                  QStringLiteral("file"));
    QCommandLineOption logFileOpt(QStringList{QStringLiteral("log-file")},
                                  QStringLiteral("Append logs to this file."),
                                  QStringLiteral("file"));
    QCommandLineOption configOpt(QStringList{QStringLiteral("config")},
                                 QStringLiteral("Use this INI config file for QSettings."),
                                 QStringLiteral("file"));
    parser.addOption(portOpt);
    parser.addOption(detachOpt);
    parser.addOption(bindOpt);
    parser.addOption(dbHostOpt);
    parser.addOption(dbPortOpt);
    parser.addOption(dbUserOpt);
    parser.addOption(dbPassOpt);
    parser.addOption(dbNameOpt);
    parser.addOption(pidFileOpt);
    parser.addOption(logFileOpt);
    parser.addOption(configOpt);
    parser.process(a);

    // Optional: set up file logging
    static QFile s_logFile;
    static QMutex s_logMutex;
    auto installLogFile = [&](const QString &path){
        if (path.isEmpty()) return;
        QFileInfo fi(path);
        if (!fi.path().isEmpty()) QDir().mkpath(fi.path());
        s_logFile.setFileName(path);
        s_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &ctx, const QString &msg){
            Q_UNUSED(ctx);
            QString level;
            switch (type) {
                case QtDebugMsg: level = QStringLiteral("DEBUG"); break;
                case QtInfoMsg: level = QStringLiteral("INFO"); break;
                case QtWarningMsg: level = QStringLiteral("WARN"); break;
                case QtCriticalMsg: level = QStringLiteral("ERROR"); break;
                case QtFatalMsg: level = QStringLiteral("FATAL"); break;
            }
            const QString line = QStringLiteral("%1 [%2] %3\n")
                    .arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODate))
                    .arg(level, msg);
            QMutexLocker locker(&s_logMutex);
            if (s_logFile.isOpen()) {
                s_logFile.write(line.toUtf8());
                s_logFile.flush();
            }
            if (type == QtFatalMsg) abort();
        });
    };
    if (parser.isSet(logFileOpt)) {
        installLogFile(parser.value(logFileOpt));
    } else {
        const QByteArray envLog = qgetenv("LOG");
        if (!envLog.isEmpty()) installLogFile(QString::fromUtf8(envLog));
    }

    // Choose settings source
    std::unique_ptr<Nutmeg::Settings> settingsPtr;
    if (parser.isSet(configOpt)) {
        settingsPtr.reset(new Nutmeg::Settings(parser.value(configOpt)));
    } else {
        settingsPtr.reset(new Nutmeg::Settings());
    }
    Nutmeg::Settings &s = *settingsPtr.get();

    // Determine WebSocket port with precedence: CLI > env > settings > default 8787
    auto parsePort = [](const QString &val, quint16 &out){
        bool ok = false;
        int p = val.toInt(&ok);
        if (ok && p > 0 && p <= 65535) { out = static_cast<quint16>(p); return true; }
        return false;
    };

    quint16 wsPort = 8787;
    wsPort = static_cast<quint16>(qvariant_cast<uint>(s.value("websocket/port", wsPort)));
    const QByteArray envPort = qgetenv("NUTMEG_WS_PORT");
    if (!envPort.isEmpty()) {
        parsePort(QString::fromUtf8(envPort), wsPort);
    }
    if (parser.isSet(portOpt)) {
        const QString cliPort = parser.value(portOpt);
        if (!parsePort(cliPort, wsPort)) {
            qCritical() << "Invalid port provided:" << cliPort;
            return 2;
        }
    }

    // Determine bind address with precedence: CLI > settings > default Any
    auto parseBind = [](const QString &str, QHostAddress &out)->bool{
        const QString bindStr = str.trimmed();
        if (bindStr.compare(QStringLiteral("localhost"), Qt::CaseInsensitive) == 0) {
            out = QHostAddress::LocalHost;
            return true;
        }
        if (bindStr == QStringLiteral("*") || bindStr.compare(QStringLiteral("any"), Qt::CaseInsensitive) == 0) {
            out = QHostAddress::Any;
            return true;
        }
        QHostAddress tmp;
        if (!tmp.setAddress(bindStr)) return false;
        out = tmp; return true;
    };

    QHostAddress bindAddr = QHostAddress::Any;
    const QString sBind = s.value("websocket/bind", QStringLiteral("0.0.0.0")).toString();
    parseBind(sBind, bindAddr); // ignore failure, keep Any
    if (parser.isSet(bindOpt)) {
        if (!parseBind(parser.value(bindOpt), bindAddr)) {
            qCritical() << "Invalid bind address:" << parser.value(bindOpt);
            return 2;
        }
    }

    // Detach if requested: re-exec without --detach
    if (parser.isSet(detachOpt)) {
        QStringList args = QCoreApplication::arguments();
        // Remove executable path
        if (!args.isEmpty()) args.pop_front();
        // Filter out -d/--detach
        QStringList filtered;
        for (int i = 0; i < args.size(); ++i) {
            const QString &arg = args.at(i);
            if (arg == QStringLiteral("-d") || arg == QStringLiteral("--detach")) {
                continue;
            }
            filtered << arg;
        }
        qint64 childPid = 0;
        const bool started = QProcess::startDetached(QCoreApplication::applicationFilePath(), filtered, QDir::currentPath(), &childPid);
        if (!started) {
            qCritical() << "Failed to detach (start background process).";
            return 3;
        }
        qInfo() << "nmbroker started in background with PID" << childPid;
        return 0;
    }

    // Resolve DB connection parameters with precedence: CLI > env > settings
    QString dbHost = s.getServer();
    uint dbPort = s.getPort();
    QString dbUser = s.getUsername();
    QString dbPass = s.getPassword();
    QString dbName = s.getDatabaseName();

    const QByteArray envDbHost = qgetenv("NUTMEG_DB_HOST");
    const QByteArray envDbPort = qgetenv("NUTMEG_DB_PORT");
    const QByteArray envDbUser = qgetenv("NUTMEG_DB_USER");
    const QByteArray envDbPass = qgetenv("NUTMEG_DB_PASSWORD");
    const QByteArray envDbName = qgetenv("NUTMEG_DB_NAME");
    if (!envDbHost.isEmpty()) dbHost = QString::fromUtf8(envDbHost);
    if (!envDbPort.isEmpty()) {
        quint16 tmp = 0; if (parsePort(QString::fromUtf8(envDbPort), tmp)) dbPort = tmp;
    }
    if (!envDbUser.isEmpty()) dbUser = QString::fromUtf8(envDbUser);
    if (!envDbPass.isEmpty()) dbPass = QString::fromUtf8(envDbPass);
    if (!envDbName.isEmpty()) dbName = QString::fromUtf8(envDbName);
    if (parser.isSet(dbHostOpt)) dbHost = parser.value(dbHostOpt);
    if (parser.isSet(dbPortOpt)) {
        quint16 tmp = 0; if (!parsePort(parser.value(dbPortOpt), tmp)) { qCritical() << "Invalid db-port"; return 2; }
        dbPort = tmp;
    }
    if (parser.isSet(dbUserOpt)) dbUser = parser.value(dbUserOpt);
    if (parser.isSet(dbPassOpt)) dbPass = parser.value(dbPassOpt);
    if (parser.isSet(dbNameOpt)) dbName = parser.value(dbNameOpt);

    // Establish default DB connection used by all models/queries (non-fatal on failure)
    qInfo() << "DB config:" << dbHost << ":" << dbPort << ", db=" << dbName << ", user=" << dbUser;
    Nutmeg::DatabaseConnection db;
    db.setServer(dbHost);
    db.setPort(dbPort);
    db.setDatabaseName(dbName);
    db.setUsername(dbUser);
    db.setPassword(dbPass);
    if (!db.Connect()) {
        qWarning() << "Database connection failed; starting broker anyway. DB-backed actions will return errors.";
    }

    Nutmeg::WebSocketServer wsServer(bindAddr, wsPort);
    if (!wsServer.isListening()) {
        qCritical() << "Unable to start WebSocket server on port" << wsPort;
        return 1;
    }

    // Write PID file if requested
    if (parser.isSet(pidFileOpt)) {
        const QString pidPath = parser.value(pidFileOpt);
        QFileInfo fi(pidPath);
        if (!fi.path().isEmpty()) QDir().mkpath(fi.path());
        QFile pf(pidPath);
        if (pf.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream ts(&pf);
            ts << QCoreApplication::applicationPid() << '\n';
            pf.close();
        } else {
            qWarning() << "Failed to write pidfile at" << pidPath;
        }
    }

    qInfo() << "nmbroker ready on" << (bindAddr == QHostAddress::Any ? QStringLiteral("0.0.0.0") : bindAddr.toString())
            << ":" << wsPort;
    return a.exec();
}
