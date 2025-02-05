#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimeZone>

#include "property.h"
#include "logger.h"

namespace Nutmeg {

class DatabaseConnection
{
    public:
    DatabaseConnection();
    ~DatabaseConnection();

    // Database Setup
    Property(getServer, setServer) QString server;
    Property(getDatabaseName, setDatabaseName) QString databasename;
    Property(getPort, setPort) uint port;
    Property(getUsername, setUsername) QString username;
    Property(getPassword, setPassword) QString password;
    Property(getTimeZone, setTimeZone) QTimeZone timeZone;
    ReadOnlyProperty(getSuccess) bool success;
    ReadOnlyProperty(getDatabase) const QSqlDatabase& database;
    ReadOnlyProperty(getLastError) const QString& lastError;

    void setServer(const QString &val) { mServer = val; }
    void setDatabaseName(const QString &val) { mDatabaseName = val; }
    void setPort(uint val);
    void setUsername(const QString &val) { mUsername = val; }
    void setPassword(const QString &val) { mPassword = val; }
    void setTimeZone(const QTimeZone& newtz);
    const QString getServer(void) const { return mServer; }
    const QString getDatabaseName(void) const { return mDatabaseName; }
    uint getPort(void) const { return mPort; }
    const QString getUsername(void) const { return mUsername; }
    const QString getPassword(void) const { return mPassword; }
    const QTimeZone& getTimeZone(void) const {return mTimeZone; }
    const QSqlDatabase& getDatabase(void) const { return mDb; }
    const QString &getLastError(void) const {return mLastError;}

    bool Connect(void);

private:
    QSqlDatabase mDb;

    // Server setup
    QString mServer;
    uint mPort = 0;
    QString mDatabaseName;
    QString mUsername;
    QString mPassword;
    QTimeZone mTimeZone;
    QString mLastError;
    bool mSuccess;
};

} // namespace Nutmeg

#endif // DATABASECONNECTION_H
