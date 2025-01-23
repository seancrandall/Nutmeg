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
    FlagClass(String camelCase);

    ReadOnlyProperty(getFlagClassId) Key flagclassId;
    Property(getCamelCase, setCamelCase) String camelCase;
    Property(getLabel, setLabel) String label;
    Property(getDescription, setDescription) String description;
    ReadOnlyProperty(getAssociatedObjects) QList<Key> associatedObjects;
    ReadOnlyProperty(getValid) bool valid;

    Key getFlagClassId(void) { return mFlagClassId; }

    String getCamelCase(void) { return mCamelCase; }

    void setCamelCase(String newCamelCase);

    String getLabel(void) { return mLabel; }

    void setLabel(String newLabel);

    String getDescription(void) { return mDescription; }

    void setDescription(String newDescription);

    bool getValid(void) { return mValid; }

  protected:
    Key mFlagClassId;
    String mCamelCase;
    String mLabel;
    String mDescription;

    QList<Key> mAssociatedObjects;

    bool mValid = false;
};

} // namespace Nutmeg

#endif // NUTMEG_FLAGCLASS_H
