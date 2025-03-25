#include "flagclass.h"

namespace Nutmeg
{

FlagClass::FlagClass(QString camelCase)
{
    mDat = Nutdb::GetFlagClass(camelCase);
}

FlagClass::FlagClass(Key id)
{
    mDat = Nutdb::GetFlagClass(id);
}

void FlagClass::setCamelCase(QString newCamelCase)
{
    mDat.CamelCase = newCamelCase;
    Nutdb::UpdateField("FlagClass", "CamelCase", mDat.FlagClassId, newCamelCase);
}

void FlagClass::setLabel(QString newLabel)
{
    mDat.Label = newLabel;
    Nutdb::UpdateField("FlagClass", "Label", mDat.FlagClassId, newLabel);
}

void FlagClass::setDescription(QString newDescription)
{
    mDat.Description = newDescription;
    Nutdb::UpdateField("FlagClass", "Description", mDat.FlagClassId, newDescription);
}

} // namespace Nutmeg
