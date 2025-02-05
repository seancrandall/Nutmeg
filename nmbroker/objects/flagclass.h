#ifndef NUTMEG_FLAGCLASS_H
#define NUTMEG_FLAGCLASS_H

#include "dbaccess/nutdb.h"
#include "nutmeg.h"
#include "property.h"

namespace Nutmeg
{

class FlagClass
{
  public:
    FlagClass(QString camelCase);
    FlagClass(Key id);
    ~FlagClass() = default;

    ReadOnlyProperty(getFlagClassId) Key flagclassId;
    Property(getCamelCase, setCamelCase) QString camelCase;
    Property(getLabel, setLabel) QString label;
    Property(getDescription, setDescription) QString description;

    Key getFlagClassId(void) { return mDat.FlagClassId; }

    String getCamelCase(void) { return mDat.CamelCase; }

    void setCamelCase(String newCamelCase);

    String getLabel(void) { return mDat.Label; }

    void setLabel(String newLabel);

    String getDescription(void) { return mDat.Description; }

    void setDescription(String newDescription);


  protected:
      FlagClassData mDat;

    //QList<Key> mAssociatedObjects;
};

} // namespace Nutmeg

#endif // NUTMEG_FLAGCLASS_H
