#include "settings.h"

namespace Nutmeg
{

Settings::Settings(QObject *parent) : QSettings("DockPilot Software", "nutmeg", parent)
{
    setDefaultFormat(QSettings::IniFormat);

    beginGroup("database");
    mServer = value("server", DEFAULT_SERVER).toString();
    mPort = value("port", DEFAULT_PORT).toUInt();
    mDatabaseName = value("database_name", DEFAULT_DATABASE_NAME).toString();
    mUsername = value("username", DEFAULT_USERNAME).toString();
    mPassword = value("password", DEFAULT_PASSWORD).toString();
    endGroup();

    beginGroup("general");
    mAirUrl = value("air_url", DEFAULT_AIR_URL).toString();
    mMaxLogLines = value("max_log_lines", DEFAULT_MAX_LOG_LINES).toUInt();
    mPreferredLogLines = value("preferred_log_lines", DEFAULT_PREFERRED_LOG_LINES).toUInt();
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

    //If any values are missing from the config file, this will write the defaults to that file
    populate();
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

const QString Nutmeg::Settings::getAirUrl()
{
    return mAirUrl = value("general/air_url").toString();
}

void Nutmeg::Settings::setAirUrl(const QString newurl)
{
    mAirUrl = newurl;
    setValue("general/air_url", newurl);
}

unsigned int Settings::getMaxLogLines()
{
    return mMaxLogLines = value("general/max_log_lines").toUInt();
}

void Settings::setMaxLogLines(unsigned int newval)
{
    mMaxLogLines = newval;
    setValue("general/max_log_lines", newval);
}

unsigned int Settings::getPreferredLogLines()
{
    return mPreferredLogLines = value("general/preferred_log_lines").toUInt();
}

void Settings::setPreferredLogLines(unsigned int newval)
{
    mPreferredLogLines = newval;
    setValue("general/preferred_log_lines", newval);
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

    objectCacheSize = mObjectCacheSize;
    flagsCacheSize = mFlagsCacheSize;
    tagsCacheSize = mTagsCacheSize;
    deadlinesCacheSize = mDeadlinesCacheSize;

    QSettings::sync();
}

} // namespace Nutmeg
