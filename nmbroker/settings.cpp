#include "settings.h"

namespace Nutmeg
{

Settings::Settings(QObject *parent)
    : QSettings("DockPilot Software", "nutmeg", parent)
    , mCompanyName("DockPilot Software")
    , mSoftwareName("Nutmeg Docket Minder")
{

    setDefaultFormat(QSettings::IniFormat);

    beginGroup("database");
    mServer = value("server", DEFAULT_SERVER).toString();
    mPort = value("port", DEFAULT_PORT).toUInt();
    mDatabaseName = value("database_name", DEFAULT_DATABASE_NAME).toString();
    mUsername = value("username", DEFAULT_USERNAME).toString();
    mPassword = value("password", DEFAULT_PASSWORD).toString();
    endGroup();

    beginGroup("preferences");
    mAirUrl = value("air_url", DEFAULT_AIR_URL).toString();
    mMaxLogLines = value("max_log_lines", DEFAULT_MAX_LOG_LINES).toUInt();
    mPreferredLogLines = value("preferred_log_lines", DEFAULT_PREFERRED_LOG_LINES).toUInt();
    mIcsSaveLocation = value("preferences/ics_save_location", DEFAULT_ICS_SAVE_LOCATION).toString();
    endGroup();

    beginGroup("documents");
    mDocsLocation = value("docs_location", DEFAULT_DOCS_LOCATION).toString();
    mMaxFilenameLength = value("max_docname_length", DEFAULT_MAX_FILENAME_LENGTH).toUInt();
    endGroup();

    beginGroup("display");
    mFontFamily = value("font_family", DEFAULT_FONT_FAMILY).toString();
    mFontSize = value("font_size", DEFAULT_FONT_SIZE).toUInt();
    endGroup();

    beginGroup("cache");
    mObjectCacheSize = value("object_cache_size", DEFAULT_OBJECT_CACHE_SIZE).toUInt();
    mFlagsCacheSize = value("flags_cache_size", DEFAULT_FLAGS_CACHE_SIZE).toUInt();
    mTagsCacheSize = value("tags_cache_size", DEFAULT_TAGS_CACHE_SIZE).toUInt();
    mDeadlinesCacheSize = value("deadlines_cache_size", DEFAULT_DEADLINES_CACHE_SIZE).toUInt();
    endGroup();

    beginGroup("user_data");
    mFirstName = value("first_name", DEFAULT_FIRST_NAME).toString();
    mLastName = value("last_name", DEFAULT_LAST_NAME).toString();
    mEmail = value("email", DEFAULT_EMAIL).toString();
    endGroup();

    //If any values are missing from the config file, this will write the defaults to that file
    populate();
}

void Settings::setServer(QString val)
{
    setValue("database/server", val);
    mServer = val;
}

void Settings::setDatabaseName(QString val)
{
    setValue("database/database_name", val);
    mDatabaseName = val;
}

void Settings::setFontFamily(QString val)
{
    setValue("display/font_family", val);
    mFontFamily = val;
}

void Settings::setFontSize(int val)
{
    setValue("display/font_size", val);
    mFontSize = val;
}

QByteArray Settings::getWindowGeometry()
{
    QByteArray val = value("geometry/window_geometry").toByteArray();
    return val;
}

void Settings::setWindowGeometry(const QByteArray &geometry)
{
    setValue("geometry/window_geometry", geometry);
}

QByteArray Settings::getWindowState()
{
    QByteArray val = value("geometry/window_state").toByteArray();
    return val;
}

void Settings::setWindowState(const QByteArray &state)
{
    setValue("geometry/window_state", state);
}

QByteArray Settings::getSplitterGeometry()
{
    QByteArray val = value("geometry/splitter_geometry").toByteArray();
    return val;
}

void Settings::setSplitterGeometry(const QByteArray &geometry)
{
    setValue("geometry/splitter_geometry", geometry);
}

QByteArray Settings::getSplitterState()
{
    QByteArray val = value("geometry/splitter_state").toByteArray();
    return val;
}

void Settings::setSplitterState(const QByteArray &state)
{
    setValue("geometry/splitter_state", state);
}

const QString Nutmeg::Settings::getAirUrl()
{
    return mAirUrl = value("preferences/air_url").toString();
}

void Nutmeg::Settings::setAirUrl(const QString newurl)
{
    mAirUrl = newurl;
    setValue("preferences/air_url", newurl);
}

const QString Settings::getIcsSaveLocation()
{
    return mIcsSaveLocation = value("preferences/ics_save_location").toString();
}

void Settings::setIcsSaveLocation(const QString &newloc)
{
    mIcsSaveLocation = newloc;
    setValue("preferences/ics_save_location", newloc);
}

unsigned int Settings::getMaxLogLines()
{
    return mMaxLogLines = value("preferences/max_log_lines").toUInt();
}

void Settings::setMaxLogLines(unsigned int newval)
{
    mMaxLogLines = newval;
    setValue("preferences/max_log_lines", newval);
}

unsigned int Settings::getPreferredLogLines()
{
    return mPreferredLogLines = value("preferences/preferred_log_lines").toUInt();
}

void Settings::setPreferredLogLines(unsigned int newval)
{
    mPreferredLogLines = newval;
    setValue("preferences/preferred_log_lines", newval);
}

void Settings::setObjectCacheSize(uint newsize)
{
    mObjectCacheSize = newsize;
    setValue("cache/object_cache_size", newsize);
}

void Settings::setFlagsCacheSize(uint newsize)
{
    mFlagsCacheSize = newsize;
    setValue("cache/flags_cache_size", newsize);
}

void Settings::setTagsCacheSize(uint newsize)
{
    mTagsCacheSize = newsize;
    setValue("cache/tags_cache_size", newsize);
}

void Settings::setDeadlinesCacheSize(uint newsize)
{
    mDeadlinesCacheSize = newsize;
    setValue("cache/deadlines_cache_size", newsize);
}

void Settings::populate()
{
    server = mServer;
    port = mPort;
    databaseName = mDatabaseName;
    username = mUsername;
    password = mPassword;

    docsLocation = mDocsLocation;
    maxFilenameLength = mMaxFilenameLength;

    fontFamily = mFontFamily;
    fontSize = mFontSize;

    airUrl = mAirUrl;
    maxLogLines = mMaxLogLines;
    preferredLogLines = mPreferredLogLines;
    icsSaveLocation = mIcsSaveLocation;

    objectCacheSize = mObjectCacheSize;
    flagsCacheSize = mFlagsCacheSize;
    tagsCacheSize = mTagsCacheSize;
    deadlinesCacheSize = mDeadlinesCacheSize;

    firstName = mFirstName;
    lastName = mLastName;
    email = mEmail;

    QSettings::sync();
}

} // namespace Nutmeg
