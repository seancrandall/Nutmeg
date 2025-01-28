#include "object.h"

namespace Nutmeg
{

Object::Object(Key id) //: QObject(parent)
{
    Object* cachedObject = cache<Object>::getObjectFromCache(id, &Object::GetObject);
    if (cachedObject) {
        // If we find the object in cache, copy its state
        *this = *cachedObject;
        mObjectIsNull = false;
    } else {
        // If not in cache, initialize normally
        InitializeObject(id);
    }
}

// Static method
Object* Object::GetObject(Key id) {
    Object* obj = new Object(id); // Create a new object to fetch from DB
    if (obj->mObjectIsNull) {
        delete obj; // Clean up if initialization failed
        return nullptr;
    }
    return obj;
}

Object::Object() //: QObject(parent)
{
    InitializeObject(0);
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
    return mErrors; // Return a const reference to the vector
}


const QHash<QString, Flag> Object::getObjectFlags()
{
    mFlags = QHash<QString, Flag>();
    QStringList flagStrings = Nutdb::GetObjectFlags(mDat.ObjectId);

    if (flagStrings.size() > 0)
    {
        // for ( const auto& i : flagStrings )
        for (auto i = 0; i < flagStrings.size(); i++)
        {
            mFlags.insert(flagStrings[i], Flag(mDat.ObjectId, flagStrings[i]));
        }
    }
    return mFlags;
}

const QHash<QString, Tag> Object::getObjectTags()
{
    QStringList tagStrings = Nutdb::GetObjectTags(mDat.ObjectId);
    mTags = QHash<QString, Tag>();

    if (tagStrings.size() > 0)
    {
        for (auto i = 0; i < tagStrings.size(); i++)
        {
            mTags.insert(tagStrings[i], Tag(tagStrings[i]));
        }
    }
    return mTags;
}

const QList<Key> Object::getObjectDocuments()
{
    mDocuments = QList<Key>();
    mDocuments = Nutdb::GetObjectDocuments(mDat.ObjectId);
    return mDocuments;
}

const QList<Key> Object::getObjectAppointments()
{
    mAppointments = QList<Key>();
    mAppointments = Nutdb::GetObjectAppointments(mDat.ObjectId);
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
    bool result = (bool) Nutdb::ClearFlag(mDat.ObjectId, camelCase); getObjectFlags();
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

bool Object::InitializeObject(Key newid)
{
    if(newid <= 0)
    {
        mObjectIsNull = true;
        primaryKey = 0;
        mDat = ObjectData();
        return false;
    }

    mDat = Nutdb::GetObject(newid);
    primaryKey = mDat.ObjectId;
    if(mDat.ObjectId == 0)
    {
        return InitializeObject(0);
    }

    mObjectType = Nutdb::GetObjectTypeString(mDat.fkObjectType);
    if(mObjectType == QString()) return false;

    getObjectFlags();
    getObjectTags();
    getObjectDocuments();
    return true;
}

bool Object::WriteAbstractValue(QString table, String fieldName, String value)
{
    return Nutdb::UpdateField(table, fieldName, primaryKey, value);
}

bool Object::WriteString(QString table, String field, String value)
{
    return WriteAbstractValue(table, field, value);
}

bool Object::WriteKey(QString table, String field, Key value)
{
    QString strval = QString::number(value);
    return WriteAbstractValue(table, field, strval);
}

bool Object::WriteDate(QString table, String field, Date date)
{
    QString strval = date.toString("yyyy-MM-dd");
    return WriteAbstractValue(table, field, strval);
}

bool Object::WriteDateTime(QString table, String field, DateTime dtime)
{
    QString strval = dtime.toString("yyyy-MM-dd HH:mm:ss");
    return WriteAbstractValue(table, field, strval);
}

bool Object::WriteBoolean(QString table, String field, bool value)
{
    QString strval = QString::number((int)value);
    return WriteAbstractValue(table, field, strval);
}

bool Object::WriteValue(QString table, QString field, QVariant value)
{
    QString strval = value.toString();
    return WriteAbstractValue(table, field, strval);
}

} // namespace Nutmeg
