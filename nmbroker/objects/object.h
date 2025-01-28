#ifndef NUTMEG_OBJECT_H
#define NUTMEG_OBJECT_H

#include <QObject>

#include "dbaccess/nutdb.h"
#include "flag.h"
#include "nutmeg.h"
#include "property.h"
#include "tag.h"
#include "cache.h"

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
class Object : public QObject
{
    Q_OBJECT
  public:
    explicit Object(Key object_id, QObject *parent = nullptr);
    explicit Object(QObject *parent = nullptr);
    explicit Object(QString objectType,
                    QObject *parent = nullptr); /// Should almost(?) never be called. Instead,
                                                /// insert the correct higher-level object

    Property(getId, slotSetId) Key ObjectId;
    Property(getfkObjectType, setfkObjectType) Key fkObjectType;
    Property(getObjectType, setObjectType) String objectType;
    ReadOnlyProperty(getFlags) QHash<QString, Flag> flags;
    ReadOnlyProperty(getTags) QHash<QString, Tag> tags;
    ReadOnlyProperty(getObjectDocuments) QList<Key> docs;
    ReadOnlyProperty(getObjectAppointments) QList<Key> appointments;
    Property(getUpdatePolicy, setUpdatePolicy) int updatePolicy;
    ReadOnlyProperty(getObjectIsNull) bool isNull;
    ReadOnlyProperty(anyDirtyValues) bool isDirty;
    ReadOnlyProperty(getErrors) QVector<QString>& errors;
    ReadOnlyProperty(getHadError) bool hadError;

    // Getters
    virtual Key getId(void) { return mDat.ObjectId; }
    Key getfkObjectType(void) { return mDat.fkObjectType; }

    bool setfkObjectType(Key newfk);
    void holdfkObjectType(Key newfk);

    String getObjectType(void) { return mObjectType; }

    void setObjectType(String objectType);
    const QVector<QString>& getErrors(void) const;

    const QHash<QString, Flag> getObjectFlags(void);
    const QHash<QString, Tag> getObjectTags(void);
    const QList<Key> getObjectDocuments(void);
    const QList<Key> getObjectAppointments(void);

    bool SetFlag(QString camelCase);
    bool ClearFlag(QString camelCase);
    bool SetFlagValue(QString camelCase, bool value);
    bool TagObject(QString tagText);
    bool ClearTag(QString tagText);

    bool getObjectIsNull(void) {return mObjectIsNull;}
    bool anyDirtyValues(void) const;

    void commit(void) { slotCommit(); }

  public slots:
    bool slotUpdate(ObjectData dat);
    virtual bool slotSetId(Key newid);
    void slotChangeObjectType(QString newObjectType);
    virtual bool slotCommit(void);

    void slotSetFlag(QString camelCase) { SetFlag(camelCase); }
    void slotClearFlag(QString camelCase) { ClearFlag(camelCase); }
    void slotTagObject(QString tagText) { TagObject(tagText); }
    void slotClearTag(QString tagText) { ClearTag(tagText); }

  signals:

  protected:
    ObjectData mDat;
    QString mObjectType;

    QHash<QString, Flag> mFlags;
    QHash<QString, Tag> mTags;
    QList<Key> mDocuments;
    QList<Key> mAppointments;

    // QList<Document> mDocuments;
    bool InitializeObject(Key id);
    bool WriteAbstractValue(String table, String fieldName, String value);
    virtual bool WriteString(String table, String field, String value);
    virtual bool WriteKey(String table, String field, Key value);
    virtual bool WriteDate(String table, String field, Date date);
    virtual bool WriteDateTime(String table, String field, DateTime dtime);
    virtual bool WriteBoolean(String table, String field, bool value);
    virtual bool WriteValue(String table, String field, QVariant value);

    bool mObjectIsNull = true;
    bool mHadError = false;

    QHash<QString, bool> dirty;
    QVector<QString> mErrors;
    const QString objectTableName = "object";
    Key primaryKey = 0;
};

} // namespace Nutmeg

#endif // NUTMEG_OBJECT_H
