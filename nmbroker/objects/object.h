#ifndef NUTMEG_OBJECT_H
#define NUTMEG_OBJECT_H

#include <QSqlRecord>

#include "nutmeg.h"

#include "dbaccess/nutdb.h"
#include "flag.h"
#include "property.h"
#include "tag.h"
#include "cache/cache.h"
#include "structures.h"

namespace Nutmeg
{

/**
 * @brief The Object class is the base class for database objects.
 *
 * Each database object has a number of read-write properties, as well as getter and
 * setter methods. The method:
 *  ' setFoo(newvalue) ' will immediately set the value and write it to the database.
 *  Each of the declspec-type properties calls setFoo. So
 *  object.foo = 12 will call setFoo(12) and will write 12 to column foo in the database
 *
 *  There are also 'hold' methods. Calling 'holdFoo(12)' will place the value of
 *  12 into the variable foo of the data structure and set a dirty flag so we know
 *  foo is 'dirty.' It will not immediately write to the database. After calling hold
 *  methods, you can call 'commit()' to commit the updated record to the databse.
 *  Calling commit() will clear all the dirty flags.
 *
 *  After calling a set or commit, you can check bool lastWriteSuccessful to see if the
 *  last write to the db was successful. If there are errors, you can check the dbErrors
 *  property, which is a vector or time-stampd error messages.
 */
class Object
{
  public:
    explicit Object();
    explicit Object(Key id);
    explicit Object(QSqlRecord &record);
    explicit Object(QString objectType);        /// Should almost(?) never be called. Instead,
                                                /// insert the correct higher-level object
    virtual ~Object() = default;

    Property(getId, slotSetId) Key ObjectId;
    Property(getfkObjectType, setfkObjectType) Key fkObjectType;
    Property(getObjectType, setObjectType) String objectType;
    ReadOnlyProperty(getObjectFlags) QList<FlagData> flags;
    Property(getFlag, SetFlagValue) bool flag[];
    Property(getFlagData, setFlagData) FlagData flagData[];
    ReadOnlyProperty(getTags) QList<TagData> tags;
    ReadOnlyProperty(getObjectDocuments) QList<Key> docs;
    ReadOnlyProperty(getObjectAppointments) QList<Key> appointments;
    Property(getUpdatePolicy, setUpdatePolicy) int updatePolicy;
    ReadOnlyProperty(getObjectIsNull) bool isNull;
    ReadOnlyProperty(anyDirtyValues) bool isDirty;
    ReadOnlyProperty(getErrors) QVector<QString>& errors;
    ReadOnlyProperty(getHadError) bool hadError;

    // Getters
    virtual Key getId(void) const { return mDat.ObjectId; }
    Key getfkObjectType(void) const { return mDat.fkObjectType; }

    bool setfkObjectType(Key newfk);
    void holdfkObjectType(Key newfk);

    QString getObjectType(void) const { return mObjectType; }

    void setObjectType(String objectType);
    const QVector<QString>& getErrors(void) const;

    const QList<FlagData> getObjectFlags(void) const;
    bool getFlag(const QString& camelCase) const;
    FlagData getFlagData(const QString& camelCase);
    const QList<TagData> getObjectTags(void) const;
    const QList<Key> getObjectDocuments(void) const;
    const QList<Key> getObjectAppointments(void) const;

    bool SetFlag(QString camelCase);
    bool setFlagData(QString camelCase, bool value);
    bool ClearFlag(QString camelCase);
    bool SetFlagValue(QString camelCase, bool value);
    bool TagObject(QString tagText);
    bool ClearTag(QString tagText);

    bool getObjectIsNull(void) {return mObjectIsNull;}
    bool anyDirtyValues(void) const;

    void commit(void) { Commit(); }

    bool Update(ObjectData dat);
    virtual bool SetId(Key newid);
    void ChangeObjectType(QString newObjectType);
    virtual bool Commit(void);

  protected:
    ObjectData mDat;
    QString mObjectType;


    // QList<Document> mDocuments;
    bool InitializeObject(Key id);
    bool WriteAbstractValue(String table, String fieldName, String value);
    virtual bool WriteString(String table, String field, String value);
    virtual bool WriteKey(String table, String field, Key value);
    virtual bool WriteDate(String table, String field, Date date);
    virtual bool WriteDateTime(String table, String field, DateTime dtime);
    virtual bool WriteBoolean(String table, String field, bool value);
    virtual bool WriteValue(String table, String field, QVariant value);

    /**
     * @brief checkFields Returns true if all necessary fields for the object are in the QSqlRecord. False if any fields not found
     * @return True if all fields found. False if any not found.
     */
    virtual bool checkFields(QSqlRecord &record);
    virtual void populate(QSqlRecord &record);

    bool mObjectIsNull = true;
    bool mHadError = false;

    QHash<QString, bool> dirty;
    QVector<QString> mErrors;

private:
    static std::shared_ptr<Object> GetObject(Key id);

    void FetchFlags(void);
    void FetchTags(void);
    void FetchAppointments(void);
    void FetchDocuments(void);

    QList<FlagData> mFlags;
    QHash<QString, bool> hFlagValues;
    QList<TagData> mTags;
    QList<Key> mDocuments;
    QList<Key> mAppointments;
};

} // namespace Nutmeg

#endif // NUTMEG_OBJECT_H
