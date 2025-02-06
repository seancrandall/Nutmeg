#include "object.h"
#include "logger.h"

namespace Nutmeg
{

Object::Object() //: QObject(parent)
{
    InitializeObject(0);
}

Object::Object(QSqlRecord &record)
{
    if(!checkFields(record))
    {
        InitializeObject(0);
        return;
    }

    populate(record);
    mObjectIsNull = false;
    mHadError = false;
}

Object::Object(QString objectType)
{
    Key newkey = Nutdb::InsertObject(objectType);
    if(newkey == 0)
    {
        InitializeObject(0);
        return;
    }
    else
    {
        mDat.ObjectId = newkey;
        InitializeObject(newkey);
    }
}

Object::Object(Key id) //: QObject(parent)
{
    auto& objectCache = getCache<Object>();
    if (objectCache.contains(id)) {  // Check if object is already in cache
        Object* cachedObject = *objectCache.object(id);
        if (cachedObject) {
            *this = *cachedObject;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, initialize normally
    InitializeObject(id);
}

std::shared_ptr<Object> Object::GetObject(Key id) {
    auto& objectCache = getCache<Object>();
    if (objectCache.contains(id)) {
        // Return a shared_ptr that does not manage the object's lifetime
        return std::shared_ptr<Object>(*objectCache.object(id), [](Object*){});
    }

    // If not in cache, create new Object and initialize it
    auto newObject = std::make_shared<Object>();
    if (newObject->InitializeObject(id)) {
        // Insert the raw pointer into the cache
        Object** tempPtr = new Object*[1];
        tempPtr[0] = newObject.get();
        objectCache.insert(id, tempPtr);
        delete[] tempPtr; // Clean up the temporary pointer array

        return newObject;
    } else {
        return nullptr;
    }
}

void Object::FetchFlags()
{
    mFlags = Nutdb::GetObjectFlags(mDat.ObjectId);

    for(int i = 0; i < mFlags.size(); ++i){
        QString cc = mFlags[i].CamelCase;
        bool val = mFlags[i].FlagValue;
        hFlagValues[cc] = val;
    }
}

void Object::FetchTags()
{
    mTags = Nutdb::GetObjectTags(mDat.ObjectId);
}

void Object::FetchAppointments()
{
    mAppointments = QList<Key>();
    mAppointments = Nutdb::GetObjectAppointments(mDat.ObjectId);
}

void Object::FetchDocuments()
{
    mDocuments = QList<Key>();
    mDocuments = Nutdb::GetObjectDocuments(mDat.ObjectId);
}


bool Object::setfkObjectType(Key newfk)
{
    bool result = WriteKey("object", "objectType", newfk);
    if(result) mDat.fkObjectType = newfk;
    return result;
}

void Object::holdfkObjectType(Key newfk)
{
    mDat.fkObjectType = newfk;
}

const QVector<QString>& Object::getErrors() const {
    return mErrors;
}

bool Object::getFlag(const QString &camelCase)
{
    if(!hFlagValues.contains(camelCase)){
        ClearFlag(camelCase);
        return false;
    }

    return hFlagValues[camelCase];
}

const QList<FlagData> Object::getObjectFlags() const
{
    return mFlags;
}

const QList<TagData> Object::getObjectTags() const
{

    return mTags;
}

const QList<Key> Object::getObjectDocuments() const
{
    return mDocuments;
}

const QList<Key> Object::getObjectAppointments() const
{
    return mAppointments;
}

bool Object::SetFlag(QString camelCase)
{
    bool result = (bool) Nutdb::SetFlag(mDat.ObjectId, camelCase);
    if(result)
        getObjectFlags();
    return result;
}

bool Object::ClearFlag(QString camelCase)
{
    bool result = (bool) Nutdb::ClearFlag(mDat.ObjectId, camelCase);
    getObjectFlags();
    return result;
}

bool Object::SetFlagValue(QString camelCase, bool value)
{
    bool result;
    if (value)
        result = (bool) Nutdb::SetFlag(mDat.ObjectId, camelCase);
    else
        result = (bool) Nutdb::ClearFlag(mDat.ObjectId, camelCase);

    if(result)
        getObjectFlags();
    return result;
}

bool Object::TagObject(QString tagText)
{
    bool result = (bool) Nutdb::TagObject(mDat.ObjectId, tagText);
    if(result)
        getObjectTags();

    return result;
}

bool Object::ClearTag(QString tagText)
{
    bool result = (bool) Nutdb::ClearTag(mDat.ObjectId, tagText);
    if(result)
        getObjectTags();

    return result;
}

/**
 * @brief Object::anyDirtyValues Check whether there are any "dirty" values in the data set
 * @return bool true if a dirty value is found, false otherwise
 */
bool Object::anyDirtyValues() const
{
    // Use std::any_of to check for any true values in dirtyFlags
    return std::any_of(dirty.cbegin(), dirty.cend(), [](bool value) {
        return value; // Directly checks if the value is true
    });
}

bool Object::Update(ObjectData dat) {
    bool result = Nutdb::UpdateObject(dat);
    if(!result) return false;

    dirty = QHash<QString, bool>();
    result = InitializeObject(dat.ObjectId);
    return result;
}

bool Object::SetId(Key newid)
{
    return InitializeObject(newid);
    dirty = QHash<QString, bool>();
}

void Object::ChangeObjectType(QString newObjectType)
{
    mObjectType = newObjectType;
    mDat.fkObjectType = Nutdb::GetObjectTypeId(newObjectType);
    commit();
}

bool Object::Commit() { return Update(mDat); }



bool Object::checkFields(QSqlRecord &record)
{
    bool found = (record.contains("ObjectId") &&
                  record.contains("fkObjectType"));

    return found;
}

void Object::populate(QSqlRecord &record)
{
    mDat.ObjectId = record.value("ObjectId").toUInt();
    mDat.fkObjectType = record.value("fkObjectType").toUInt();
}

bool Object::WriteAbstractValue(QString table, QString fieldName, QString value)
{
    bool success = Nutdb::UpdateField(table, fieldName, ObjectId, value);
    if (!success) {
        auto errmsg = QString("Error: Failed to update field '%1' in table '%2'").arg(fieldName).arg(table);
        Logger::LogMessage(errmsg);
        mHadError = true;
        mErrors << errmsg;
    }
    return success;
}

bool Object::WriteString(QString table, QString field, QString value)
{
    bool success = WriteAbstractValue(table, field, value);
    if (!success) {
        auto errmsg = QString("Error: Failed to write string value to field '%1' in table '%2'").arg(field).arg(table);
        Logger::LogMessage(errmsg);
        mHadError = true;
        mErrors << errmsg;
    }
    return success;
}

bool Object::WriteKey(QString table, QString field, Key value)
{
    QString strval = QString::number(value);
    bool success = WriteAbstractValue(table, field, strval);
    if (!success) {
        auto errmsg = QString("Error: Failed to write key value to field '%1' in table '%2'").arg(field).arg(table);
        Logger::LogMessage(errmsg);
        mHadError = true;
        mErrors << errmsg;
    }
    return success;
}

bool Object::WriteDate(QString table, QString field, Date date)
{
    QString strval = date.toString("yyyy-MM-dd");
    bool success = WriteAbstractValue(table, field, strval);
    if (!success) {
        auto errmsg = QString("Error: Failed to write date to field '%1' in table '%2'").arg(field).arg(table);
        Logger::LogMessage(errmsg);
        mHadError = true;
        mErrors << errmsg;
    }
    return success;
}

bool Object::WriteDateTime(QString table, QString field, DateTime dtime)
{
    QString strval = dtime.toString("yyyy-MM-dd HH:mm:ss");
    bool success = WriteAbstractValue(table, field, strval);
    if (!success) {
        auto errmsg = QString("Error: Failed to write datetime to field '%1' in table '%2'").arg(field).arg(table);
        Logger::LogMessage(errmsg);
        mHadError = true;
        mErrors << errmsg;
    }
    return success;
}

bool Object::WriteBoolean(QString table, QString field, bool value)
{
    QString strval = QString::number((int)value);
    bool success = WriteAbstractValue(table, field, strval);
    if (!success) {
        auto errmsg = QString("Error: Failed to write boolean to field '%1' in table '%2'").arg(field).arg(table);
        Logger::LogMessage(errmsg);
        mHadError = true;
        mErrors << errmsg;
    }
    return success;
}

bool Object::WriteValue(QString table, QString field, QVariant value)
{
    QString strval = value.toString();
    bool success = WriteAbstractValue(table, field, strval);
    if (!success) {
        auto errmsg = QString("Error: Failed to write value to field '%1' in table '%2'").arg(field).arg(table);
        Logger::LogMessage(errmsg);
        mHadError = true;
        mErrors << errmsg;
    }
    return success;
}

bool Object::InitializeObject(Key newid)
{
    if(newid == 0)
    {
        mObjectIsNull = true;
        mDat = ObjectData();
        return false;
    }

    mDat = Nutdb::GetObject(newid);
    if(mDat.ObjectId == 0)
    {
        mObjectIsNull = true;
        return InitializeObject(0);
    }

    mObjectType = Nutdb::GetObjectTypeString(mDat.fkObjectType);
    if(mObjectType == QString()) return false;

    mObjectIsNull = false;

    FetchFlags();
    FetchTags();
    FetchAppointments();
    FetchDocuments();

    return true;
}

} // namespace Nutmeg
