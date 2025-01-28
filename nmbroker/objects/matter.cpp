#include "matter.h"

namespace Nutmeg
{

Matter::Matter() : Nutmeg::Object{} {}

Matter::Matter(Key newid) : Nutmeg::Object()
{
    Matter* cachedMatter = cache<Matter>::getObjectFromCache(newid, &Matter::GetMatter);
    if (cachedMatter) {
        // If we find the matter in cache, copy its state
        *this = *cachedMatter;
        mObjectIsNull = false; // Assuming Matter inherits this from Object
    } else {
        // If not in cache, initialize normally
        InitializeMatter(newid);
    }
}

// Static method to fetch a Matter from the database if not in cache
Matter* Matter::GetMatter(Key id) {
    Matter* matter = new Matter(id); // This will call the constructor again, but now with cache check
    if (matter->mObjectIsNull) { // Assuming Matter inherits this from Object
        delete matter; // Clean up if initialization failed
        return nullptr;
    }
    return matter;
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
    if(mDat.MatterId == 0) return InitializeMatter(0);

    return Object::InitializeObject(mDat.MatterId);
}

Key Nutmeg::Matter::getfkParent()
{
    return mDat.fkParent;
}

} // namespace Nutmeg
