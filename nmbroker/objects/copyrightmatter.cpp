#include "copyrightmatter.h"

namespace Nutmeg
{

CopyrightMatter::CopyrightMatter() : Nutmeg::Matter{} {}

CopyrightMatter::CopyrightMatter(Key newid) : Matter(newid)
{
    auto& copyrightMatterCache = getCache<CopyrightMatter>();
    if (copyrightMatterCache.contains(newid)) {  // Check if CopyrightMatter is already in cache
        CopyrightMatter* cachedCopyrightMatter = *copyrightMatterCache.object(newid);
        if (cachedCopyrightMatter) {
            *this = *cachedCopyrightMatter;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, proceed with initialization
    InitializeCopyrightMatter(newid);
}
// Static method to fetch a CopyrightMatter from the database if not in cache
CopyrightMatter* CopyrightMatter::GetCopyrightMatter(Key id) {
    auto& copyrightMatterCache = getCache<CopyrightMatter>();
    if (copyrightMatterCache.contains(id)) {
        return *copyrightMatterCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new CopyrightMatter and initialize it
    CopyrightMatter* newCopyrightMatter = new CopyrightMatter(id);
    if (!newCopyrightMatter->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        copyrightMatterCache.insert(id, &newCopyrightMatter);
        return newCopyrightMatter;
    } else {
        delete newCopyrightMatter;  // Clean up if initialization failed
        return nullptr;
    }
}

CopyrightMatter::CopyrightMatter(String docketNumber) : Matter()
{
    Key newid = Nutdb::InsertCopyrightMatter(docketNumber);
    InitializeCopyrightMatter(newid);
}

Key CopyrightMatter::AddAuthor(String first, String last)
{
    Key personid = Nutdb::InsertPerson(first, last);
    if(personid)
        slotSetfkAuthor(personid);
    return personid;
}

bool CopyrightMatter::getIsRegistered()
{
    mIsRegistered = Nutdb::GetFlag(mDat.CopyrightMatterId, "Registered");
    Object::getObjectFlags();
    return mIsRegistered;
}

bool CopyrightMatter::SetId(Key newval)
{
    return InitializeCopyrightMatter(newval);
}

bool CopyrightMatter::slotUpdate(CopyrightMatterData dat)
{
    slotSetIsRegistered(mIsRegistered);
    bool result = Nutdb::UpdateCopyrightMatter(dat);
    if(!result) return false;

    result = Matter::Commit();
    if(!result) return false;

    return InitializeCopyrightMatter(dat.CopyrightMatterId);
}

bool CopyrightMatter::Commit()
{
    return slotUpdate(mDat);
}

bool CopyrightMatter::slotSetfkAuthor(Key newval)
{
    bool result = WriteKey(copyrightMatterTableName, "fkAuthor", newval);
    if(result) mDat.fkAuthor = newval;
    return result;
}

bool CopyrightMatter::slotSetCreated(Date newval)
{
    bool result = WriteDate(copyrightMatterTableName, "Created", newval);
    if(result) mDat.Created = newval;
    return result;
}

bool CopyrightMatter::slotSetFiled(Date newval)
{
    bool result = WriteDate(copyrightMatterTableName, "Filed", newval);
    if(result) mDat.Filed = newval;
    return result;
}

bool CopyrightMatter::slotSetRegistered(Date newval)
{
    bool result = WriteDate(copyrightMatterTableName, "Registered", newval);
    if(result) mDat.Registered = newval;
    return result;
}

bool CopyrightMatter::slotSetSerialNumber(String newval)
{
    bool result = WriteString(copyrightMatterTableName, "SerialNumber", newval);
    if(result) mDat.SerialNumber = newval;
    return result;
}

bool CopyrightMatter::slotSetRegistrationNumber(String newval)
{
    bool result = WriteString(copyrightMatterTableName, "RegistrationNumber", newval);
    if(result) mDat.RegistrationNumber = newval;
    return result;
}

bool CopyrightMatter::slotSetfkDeposit(Key newval)
{
    bool result = WriteKey(copyrightMatterTableName, "fkDeposit", newval);
    if(result) mDat.fkDeposit = newval;
    return result;
}

bool CopyrightMatter::slotSetfkWorkType(Key newval)
{
    bool result = WriteKey(copyrightMatterTableName, "fkWorkType", newval);
    if(result) mDat.fkWorkType = newval;
    return result;
}

bool CopyrightMatter::slotSetIsRegistered(bool newval)
{
    if (newval) Nutdb::SetFlag(mDat.CopyrightMatterId, "Registered");
    else Nutdb::ClearFlag(mDat.CopyrightMatterId, "Registered");

    Object::getObjectFlags();
    return true;
}

bool CopyrightMatter::InitializeCopyrightMatter(Key newid)
{
    if(newid == 0)
    {
        mDat = CopyrightMatterData();
        return InitializeMatter(0);
    }

    mDat = Nutdb::GetCopyrightMatter(newid);
    if(mDat.CopyrightMatterId == 0) return InitializeCopyrightMatter(0);

    mIsRegistered = Nutdb::GetFlag(newid, "Registered");
    return InitializeMatter(newid);
}

} // namespace Nutmeg
