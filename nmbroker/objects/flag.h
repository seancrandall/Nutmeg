#ifndef NUTMEG_FLAG_H
#define NUTMEG_FLAG_H

#include "dbaccess/nutdb.h"
#include "nutmeg.h"
#include "property.h"
#include <QObject>
// #include "structures.h"
#include "flagclass.h"

namespace Nutmeg
{

class Flag : public FlagClass
{
  public:
    explicit Flag(Key object_id, String camel_case);
    ~Flag();
    static Flag* GetFlag(Key id);  //Static method for fetching from database

    Property(getFlagValue, setFlagValue) bool value;
    ReadOnlyProperty(getObjectId) Key objectId;

    bool getFlagValue(void);
    bool setFlagValue(bool newValue);

    Key getObjectId(void) { return mObjectId; }

  signals:

  protected:
    bool mFlagValue;
    Key mObjectId;
    Key mFlagKey;
    QString mCamelCase;

    void InitializeFlag(Key flagId, QString camelCase);
};

} // namespace Nutmeg

#endif // NUTMEG_FLAG_H
