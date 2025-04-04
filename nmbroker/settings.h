#ifndef NUTMEG_SETTINGS_H
#define NUTMEG_SETTINGS_H

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>

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
#define DEFAULT_ICS_SAVE_LOCATION QStandardPaths::writableLocation(QStandardPaths::HomeLocation)


// Documents defaults
#define DEFAULT_DOCS_LOCATION "~/"
#define DEFAULT_MAX_FILENAME_LENGTH 512

// Display defaults
#define DEFAULT_FONT_FAMILY "Arial"
#define DEFAULT_FONT_SIZE 12

//Cache Defaults
#define DEFAULT_OBJECT_CACHE_SIZE 5000
#define DEFAULT_FLAGS_CACHE_SIZE 5000
#define DEFAULT_TAGS_CACHE_SIZE 5000
#define DEFAULT_DEADLINES_CACHE_SIZE 5000

//User Data Defaults
#define DEFAULT_FIRST_NAME "John"
#define DEFAULT_LAST_NAME "Doe"
#define DEFAULT_EMAIL "john.doe@example.com"

class Settings : public QSettings
{
  public:
    Settings(QObject *parent = nullptr);

    ReadOnlyProperty(getCompanyName) QString companyName;
    ReadOnlyProperty(getSoftwareName) QString softwareName;

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
    Property(getIcsSaveLocation, setIcsSaveLocation) QString icsSaveLocation;

    // Documents
    Property(getDocsLocation, setDocsLocation) QString docsLocation;
    Property(getMaxFilenameLength, setMaxFilenameLength) uint maxFilenameLength;

    // Display
    Property(getFontFamily, setFontFamily) QString fontFamily;
	Property(getFontSize, setFontSize) uint fontSize;
    Property(getSplitterState, setSplitterState) QByteArray splitterState;
    Property(getSplitterGeometry, setSplitterGeometry) QByteArray splitterGeometry;
    Property(getWindowState, setWindowState) QByteArray windowState;
    Property(getWindowGeometry, setWindowGeometry) QByteArray windowGeometry;

    //Cache
    Property(getObjectCacheSize, setObjectCacheSize) unsigned int objectCacheSize;
    Property(getFlagsCacheSize, setFlagsCacheSize) unsigned int flagsCacheSize;
    Property(getTagsCacheSize, setTagsCacheSize) unsigned int tagsCacheSize;
    Property(getDeadlinesCacheSize, setDeadlinesCacheSize) unsigned int deadlinesCacheSize;

    //User Data
    Property(getFirstName, setFirstName) QString firstName;
    Property(getLastName, setLastName) QString lastName;
    Property(getEmail, setEmail) QString email;

    // Property(getFontWeight, setFontWeight) QString fontWeight;

    void setServer(QString val);
    void setPort(uint val) { setValue("database/port", val), mPort = val; }
    void setDatabaseName(QString val);

    const QString getCompanyName(void){return mCompanyName;}
    const QString getSoftwareName(void) {return mSoftwareName;}

    void setUsername(QString val) { setValue("database/username", val), mUsername = val; }
    void setPassword(QString val) { setValue("database/password", val), mPassword = val; }
    void setDocsLocation(QString val) { setValue("documents/docs_location", val), mDocsLocation = val; }
    void setMaxFilenameLength(uint val) { setValue("documents/max_docname_length", val), mMaxFilenameLength = val; }
    void setFontFamily(QString val);
    void setFontSize(int val);
    QString getServer(void) { return mServer = value("database/server").toString(); }
    uint getPort(void) { return mPort = value("database/port").toUInt(); }
    QString getDatabaseName(void) { return mDatabaseName = value("database/database_name").toString(); }
    QString getUsername(void) { return mUsername = value("database/username").toString(); }
    QString getPassword(void) { return mPassword = value("database/password").toString(); }
    QString getDocsLocation(void) { return mDocsLocation = value("documents/docs_location").toString(); }
    uint getMaxFilenameLength(void) { return mMaxFilenameLength = value("documents/max_docname_length").toUInt(); }
    QString getFontFamily(void) { return mFontFamily = value("display/font_family").toString(); }
    uint getFontSize(void) { return mFontSize = value("display/font_size").toUInt(); }
    QByteArray getWindowGeometry(void);
    void setWindowGeometry(const QByteArray& geometry);
    QByteArray getWindowState(void);
    void setWindowState(const QByteArray& state);
    QByteArray getSplitterGeometry(void);
    void setSplitterGeometry(const QByteArray& geometry);
    QByteArray getSplitterState(void);
    void setSplitterState(const QByteArray& state);

    void setFirstName(QString val) {setValue("user_data/first_name", val); mFirstName = val; }
    QString getFirstName(void) { return mFirstName = value("user_data/first_name").toString();}
    void setLastName(QString val) {setValue("user_data/last_name", val); mLastName = val; }
    QString getLastName(void) {return mLastName = value("user_data/last_name").toString();}
    void setEmail(QString val) {setValue("user_data/email", val); mEmail = val;}
    QString getEmail(void) {return mEmail = value("user_data/email").toString();}

    const QString getAirUrl(void);
    void setAirUrl(const QString newurl);

    const QString getIcsSaveLocation(void);
    void setIcsSaveLocation(const QString &newloc);

    unsigned int getMaxLogLines(void);
    void setMaxLogLines(unsigned int newval);

    unsigned int getPreferredLogLines(void);
    void setPreferredLogLines(unsigned int newval);

    uint getObjectCacheSize(void) {return mObjectCacheSize = value("cache/object_cache_size").toUInt();}
    uint getFlagsCacheSize(void) {return mFlagsCacheSize = value("cache/flags_cache_size").toUInt();}
    uint getTagsCacheSize(void) {return mTagsCacheSize = value("cache/tags_cache_size").toUInt();}
    uint getDeadlinesCacheSize(void) {return mDeadlinesCacheSize = value("cache/deadlines_cache_size").toUInt();}

    void setObjectCacheSize(uint newsize);
    void setFlagsCacheSize(uint newsize);
    void setTagsCacheSize(uint newsize);
    void setDeadlinesCacheSize(uint newsize);

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
    QString mIcsSaveLocation;

    QString mFontFamily;
    uint mFontSize;

    //Cache sizes
    uint mObjectCacheSize;
    uint mFlagsCacheSize;
    uint mTagsCacheSize;
    uint mDeadlinesCacheSize;

    //User Data
    QString mFirstName;
    QString mLastName;
    QString mEmail;

    void populate(void);

private:
   QString mCompanyName;
    QString mSoftwareName;
};

} // namespace Nutmeg

#endif // NUTMEG_SETTINGS_H
