#include "flagclass.h"

namespace Nutmeg
{

FlagClass::FlagClass(String camelCase) : mCamelCase(camelCase)
{
    FlagClassData dat = Nutdb::GetFlagClass(camelCase);

    mFlagClassId = dat.FlagClassId;
    // mCamelCase = dat.CamelCase;
    mLabel = dat.Label;
    mDescription = dat.Description;

    // Get a list of objects associated with this flag
    TRY(mAssociatedObjects = Nutdb::GetFlagObjects(mCamelCase);)
}

void FlagClass::setCamelCase(String newCamelCase)
{
    mCamelCase = newCamelCase;
    TRY(Nutdb::UpdateField("FlagClass", "CamelCase", mFlagClassId, newCamelCase);)
}

void FlagClass::setLabel(String newLabel)
{
    mLabel = newLabel;
    TRY(Nutdb::UpdateField("FlagClass", "Label", mFlagClassId, newLabel);)
}

void FlagClass::setDescription(String newDescription)
{
    mDescription = newDescription;
    TRY(Nutdb::UpdateField("FlagClass", "Description", mFlagClassId, newDescription);)
}

} // namespace Nutmeg
