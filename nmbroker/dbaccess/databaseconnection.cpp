#include "databaseconnection.h"

namespace Nutmeg {


DatabaseConnection::DatabaseConnection()
    : mTimeZone(QTimeZone())
{
    QSqlDatabase mDb = QSqlDatabase::addDatabase("QMYSQL");
    QSqlQuery query = QSqlQuery(mDb);
    mDb.setHostName(server);
    if (port > 0)
        mDb.setPort(port); // only set port if one has been specified
    mDb.setDatabaseName(databasename);
    mDb.setUserName(username);
    mDb.setPassword(password);

    mSuccess = mDb.open();

    if (!mSuccess)
    {
        qDebug() << "Database not successfully opened. Last error: " << mDb.lastError();
        QString msg = "Database connection failed and returned error: " + mDb.lastError().text();
        Logger::LogMessage(msg);
        mLastError = msg;
        return;
    }
    else
    {
        QString logMessage = QString("Successfully opened schema %1 on %2:%3.")
        .arg(databasename,
             server,
             QString::number(port));
        Logger::LogMessage(logMessage);
#ifdef QT_DEBUG
        QString msg = QString("Found tables: ");
        QStringList tables = mDb.tables();
        for (const QString &str : tables)
        {
            msg += "\n" + str;
        }
        Logger::LogMessage(msg);
#endif
    }
    return;
}

void DatabaseConnection::setTimeZone(const QTimeZone &newtz)
{
    QSqlQuery query(mDb); // Assuming mDb is the member variable holding the QSqlDatabase

    // Convert QTimeZone to a string suitable for SQL
    QString tzString = QStringLiteral("'%1'").arg(newtz.id());

    query.prepare("SET time_zone = ?"); // Use parameterized query for better safety
    query.addBindValue(tzString); // Bind the parameter

    mSuccess = query.exec();
    if (mSuccess){
        mTimeZone = newtz;
    } else {
        QString msg = QString("Failed to set timezone to %1. Error: %2").arg(newtz.id(), query.lastError().text());
        mLastError = msg;
        Logger::LogMessage(msg);
    }
}
DatabaseConnection::~DatabaseConnection() {
    mDb.close();
}

} // namespace Nutmeg
