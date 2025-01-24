#ifndef NUTMEG_SETTINGS_H
#define NUTMEG_SETTINGS_H

#include <QDebug>
#include <QFile>
#include <QSettings>

#include "property.h"

namespace Nutmeg
{

// Database defaults
#define DEFAULT_SERVER "192.168.1.1"
#define DEFAULT_PORT "3306"
#define DEFAULT_DATABASE_NAME "Database"
#define DEFAULT_USERNAME "User"
#define DEFAULT_PASSWORD "Password"

//General Defaults
#define DEFAULT_AIR_URL "https://www.uspto.gov"
#define DEFAULT_MAX_LOG_LINES 1000
#define DEFAULT_PREFERRED_LOG_LINES 1500

// Documents defaults
#define DEFAULT_DOCS_LOCATION "~/"
#define DEFAULT_MAX_FILENAME_LENGTH 512

// Display defaults
#define DEFAULT_FONT_FAMILY "Arial"
#define DEFAULT_FONT_SIZE 12

class Settings : public QSettings
{
  public:
    Settings(QObject *parent = nullptr);

    // Database
    Property(getServer, setServer) QString server;
	Property(getPort, setPort) uint port;
    Property(getDatabaseName, setDatabaseName) QString databaseName;
    Property(getUsername, setUsername) QString username;
    Property(getPassword, setPassword) QString password;

    //General
    Property(getAirUrl, setAirUrl) QString airUrl;
    Property(getMaxLogLines, setMaxLogLines) unsigned int maxLogLines;
    Property(getPreferredLogLines, setPreferredLogLines) unsigned int preferredLogLines;

    // Documents
    Property(getDocsLocation, setDocsLocation) QString docsLocation;
    Property(getMaxFilenameLength, setMaxFilenameLength) uint maxFilenameLength;

    // Display
    Property(getFontFamily, setFontFamily) QString fontFamily;
	Property(getFontSize, setFontSize) uint fontSize;

    // Property(getFontWeight, setFontWeight) QString fontWeight;

    void setServer(QString val)
    {
        setValue("database/server", val);
        mServer = val;
    }

    void setPort(uint val) { setValue("database/port", val), mPort = val; }

    void setDatabaseName(QString val)
    {
        setValue("database/database_name", val);
        mDatabaseName = val;
    }

    void setUsername(QString val) { setValue("database/username", val), mUsername = val; }

    void setPassword(QString val) { setValue("database/password", val), mPassword = val; }

    void setDocsLocation(QString val) { setValue("documents/docs_location", val), mDocsLocation = val; }

    void setMaxFilenameLength(uint val) { setValue("documents/max_docname_length", val), mMaxFilenameLength = val; }

    void setFontFamily(QString val)
    {
        setValue("display/font_family", val);
        mFontFamily = val;
    }

    void setFontSize(int val)
    {
        setValue("display/font_size", val);
        mFontSize = val;
    }

    QString getServer(void) { return mServer = value("database/server").toString(); }

    uint getPort(void) { return mPort = value("database/port").toUInt(); }

    QString getDatabaseName(void) { return mDatabaseName = value("database/database_name").toString(); }

    QString getUsername(void) { return mUsername = value("database/username").toString(); }

    QString getPassword(void) { return mPassword = value("database/password").toString(); }

    QString getDocsLocation(void) { return mDocsLocation = value("documents/docs_location").toString(); }

    uint getMaxFilenameLength(void) { return mMaxFilenameLength = value("documents/max_docname_length").toUInt(); }

    QString getFontFamily(void) { return mFontFamily = value("display/font_family").toString(); }

    uint getFontSize(void) { return mFontSize = value("display/font_size").toUInt(); }

    const QString getAirUrl(void);
    void setAirUrl(const QString newurl);

    unsigned int getMaxLogLines(void);
    void setMaxLogLines(unsigned int newval);

    unsigned int getPreferredLogLines(void);
    void setPreferredLogLines(unsigned int newval);

  protected:
    QString mServer;
    uint mPort;
    QString mUsername;
    QString mPassword;
    QString mDatabaseName;
    QString mDocsLocation;
    uint mMaxFilenameLength;

    QString mAirUrl;
    unsigned int mMaxLogLines,
        mPreferredLogLines;

    QString mFontFamily;
    uint mFontSize;

    void populate(void);
};

} // namespace Nutmeg

#endif // NUTMEG_SETTINGS_H
