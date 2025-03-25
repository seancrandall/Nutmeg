#include "flag.h"

namespace Nutmeg
{

Flag::Flag(Key objectId, String camelCase) : FlagClass(camelCase)
{
    mCamelCase = camelCase;
    mObjectId = objectId;
    InitializeFlag(objectId, camelCase);
}

Flag::~Flag() {}

bool Flag::getFlagValue()
{
    mFlagValue = Nutdb::GetFlag(mObjectId, mCamelCase);
    return mFlagValue;
}

bool Flag::setFlagValue(bool newValue)
{
    if (newValue)
        Nutdb::SetFlag(mObjectId, mCamelCase);
    else
        Nutdb::ClearFlag(mObjectId, mCamelCase);

    // return nut->query.isValid();
    return true;
}

void Flag::InitializeFlag(Key objectId, QString camelCase)
{
    mFlagValue = Nutdb::GetFlag(objectId, camelCase);
}

} // namespace Nutmeg
