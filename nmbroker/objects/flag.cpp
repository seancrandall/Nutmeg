#include "flag.h"

namespace Nutmeg
{

Flag::Flag(Key object_id, String camel_case) : FlagClass(camel_case)
{
    mObjectId = object_id;
    mFlagValue = Nutdb::GetFlag(mObjectId, mCamelCase);
    // mValid = nut->query.isValid();
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

} // namespace Nutmeg
