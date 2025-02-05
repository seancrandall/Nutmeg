#include "databaseconnection.h"
#include "qsqlerror.h"

namespace Nutmeg {


DatabaseConnection::DatabaseConnection()
    : mTimeZone(QTimeZone())
    , mSuccess(false)
{
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
        mLastError = QString();
        mTimeZone = newtz;
    } else {
        QString msg = QString("Failed to set timezone to %1. Error: %2").arg(newtz.id(), query.lastError().text());
        mLastError = msg;
        Logger::LogMessage(msg);
    }
}

bool DatabaseConnection::Connect()
{
    //If already open, cleanup and reconnect
    if(mDb.isOpen())
    {
        mDb.close();
        QString connectionName = mDb.connectionName();
        QSqlDatabase::removeDatabase(connectionName);
        mSuccess = Connect();
        return mSuccess;
    }

    mDb = QSqlDatabase::addDatabase("QMYSQL");

    // Set connection parameters
    mDb.setHostName(mServer);
    if (mPort > 0)
        mDb.setPort(mPort);
    mDb.setDatabaseName(mDatabaseName);
    mDb.setUserName(mUsername);
    mDb.setPassword(mPassword);

    // Attempt to open the connection
    mSuccess = mDb.open();

    if (!mSuccess)
    {
        qDebug() << "Database not successfully opened. Last error: " << mDb.lastError();
        QString msg = "Database connection failed and returned error: " + mDb.lastError().text();
        Logger::LogMessage(msg);
        mLastError = msg;
        return false; // Indicate failure
    }
    else
    {
        QString logMessage = QString("Successfully opened schema %1 on %2:%3.")
        .arg(mDatabaseName, mServer, QString::number(mPort));
        Logger::LogMessage(logMessage);
        mLastError = QString();
#ifdef QT_DEBUG
        QString msg = QString("Found tables: ");
        QStringList tables = mDb.tables();
        for (const QString &str : tables)
        {
            msg += "\n" + str;
        }
        Logger::LogMessage(msg);
#endif
        return true;
    }
}

DatabaseConnection::~DatabaseConnection() {
    mDb.close();
}

void DatabaseConnection::setPort(uint val)
{
    if (val > 65535) {
        qDebug() << "Invalid port number. Port must be between 0 and 65535.";
        mLastError = "Invalid port number. Port must be between 0 and 65535.";
        mSuccess = false;
        return; // or throw an exception if you're using them
    }
    mPort = val;
}

} // namespace Nutmeg
