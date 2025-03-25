#include "matter.h"

namespace Nutmeg
{

Matter::Matter() : Nutmeg::Object{} {}

Matter::Matter(Key newid)
    : Nutmeg::Object(newid)
{
    auto& matterCache = getCache<Matter>();
    if (matterCache.contains(newid)) {  // Check if Matter is already in cache
        Matter* cachedMatter = *matterCache.object(newid);
        if (cachedMatter) {
            *this = *cachedMatter;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, initialize normally
    InitializeMatter(newid);
}

Matter* Matter::GetMatter(Key id) {
    auto& matterCache = getCache<Matter>();
    if (matterCache.contains(id)) {
        return *matterCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new Matter and initialize it
    Matter* newMatter = new Matter(id);
    if (!newMatter->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        matterCache.insert(id, &newMatter);
        return newMatter;
    } else {
        delete newMatter;  // Clean up if initialization failed
        return nullptr;
    }
}

Matter::Matter(QString docketNumber) : Nutmeg::Object()
{
    Key newid = Nutdb::InsertMatter(docketNumber);
    InitializeMatter(newid);
}

bool Matter::SetId(Key newid)
{
    return InitializeMatter(newid);
}

bool Matter::Update(MatterData dat)
{
    //First, commit the matter data
    bool result = Nutdb::UpdateMatter(dat);
    if(!result) return false;

    //Also commit object data
    result = Object::Commit();
    if(!result) return false;

    //Read back from DB
    result = InitializeMatter(dat.MatterId);
    return result;
}

bool Matter::Commit()
{
    return Update(mDat);
}

bool Matter::SetfkParent(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkParent", newfk);
    if(result) mDat.fkParent = newfk;
    return result;
}

bool Matter::SetAttorneyDocketNumber(QString newnum)
{
    bool result = WriteString("matter", "AttorneyDocketNumber", newnum);
    if(result) mDat.AttorneyDocketNumber = newnum;
    return result;
}

bool Matter::SetClientDocketNumber(QString newnum)
{
    bool result = WriteString(matterTableName, "ClientDocketNumber", newnum);
    if(result) mDat.ClientDocketNumber = newnum;
    return result;
}

bool Matter::SetTitle(QString newtitle)
{
    bool result = WriteString(matterTableName, "Title", newtitle);
    if(result) mDat.Title = newtitle;
    return result;
}

bool Matter::SetfkClient(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkClient", newfk);
    if(result) mDat.fkClient = newfk;
    return result;
}

bool Matter::SetfkAssigningFirm(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkAssigningFirm", newfk);
    if(result) mDat.fkAssigningFirm = newfk;
    return result;
}

bool Matter::SetfkDefaultWorkAttorney(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkDefaultWorkAttorney", newfk);
    if(result) mDat.fkDefaultWorkAttorney = newfk;
    return result;
}

bool Matter::SetfkDefaultParalegal(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkDefaultParalegal", newfk);
    if(result) mDat.fkDefaultParalegal = newfk;
    return result;
}

bool Matter::SetfkKeyDocument(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkKeyDocument", newfk);
    if(result) mDat.fkKeyDocument = newfk;
    return result;
}

bool Matter::SetfkMatterJurisdiction(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkMatterJurisdiction", newfk);
    if(result) mDat.fkMatterJurisdiction = newfk;
    return result;
}

bool Matter::SetOldId(Key newold)
{
    bool result = WriteKey(matterTableName, "OldMatterId", newold);
    if(result) mDat.OldMatterId = newold;
    return result;
}

bool Matter::InitializeMatter(Key newid)
{
    if(newid == 0)
    {
        mDat = MatterData();
        return InitializeObject(0);
    }

    mDat = Nutdb::GetMatter(newid);
    if(mDat.MatterId == 0)
        return InitializeMatter(0);
    else return true;

    //return Object::InitializeObject(mDat.MatterId);
    //return true;
}

Key Nutmeg::Matter::getfkParent() const
{
    return mDat.fkParent;
}

} // namespace Nutmeg
