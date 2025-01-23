#include "matter.h"

namespace Nutmeg
{

Matter::Matter(QObject *parent) : Nutmeg::Object{parent} {}

Matter::Matter(Key newid, QObject *parent) : Nutmeg::Object(parent)
{
    InitializeMatter(newid);
}

// Matter::Matter(const QSqlRecord matterRecord, QObject *parent)
//     : Nutmeg::Object(parent)
// {

// }

Matter::Matter(QString docketNumber, QObject *parent) : Nutmeg::Object(parent)
{
    Key newid = Nutdb::InsertMatter(docketNumber);
    InitializeMatter(newid);
}

bool Matter::slotSetId(Key newid)
{
    return InitializeMatter(newid);
}

bool Matter::slotUpdate(MatterData dat)
{
    //First, commit the matter data
    bool result = Nutdb::UpdateMatter(dat);
    if(!result) return false;

    //Also commit object data
    result = Object::slotCommit();
    if(!result) return false;

    //Read back from DB
    result = InitializeMatter(dat.MatterId);
    return result;
}

bool Matter::slotCommit()
{
    return slotUpdate(mDat);
}

bool Matter::slotSetfkParent(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkParent", newfk);
    if(result) mDat.fkParent = newfk;
    return result;
}

bool Matter::slotSetAttorneyDocketNumber(QString newnum)
{
    bool result = WriteString(matterTableName, "AttorneyDocketNumber", newnum);
    if(result) mDat.AttorneyDocketNumber = newnum;
    return result;
}

bool Matter::slotSetClientDocketNumber(QString newnum)
{
    bool result = WriteString(matterTableName, "ClientDocketNumber", newnum);
    if(result) mDat.ClientDocketNumber = newnum;
    return result;
}

bool Matter::slotSetTitle(QString newtitle)
{
    bool result = WriteString(matterTableName, "Title", newtitle);
    if(result) mDat.Title = newtitle;
    return result;
}

bool Matter::slotSetfkClient(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkClient", newfk);
    if(result) mDat.fkClient = newfk;
    return result;
}

bool Matter::slotSetfkAssigningFirm(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkAssigningFirm", newfk);
    if(result) mDat.fkAssigningFirm = newfk;
    return result;
}

bool Matter::slotSetfkDefaultWorkAttorney(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkDefaultWorkAttorney", newfk);
    if(result) mDat.fkDefaultWorkAttorney = newfk;
    return result;
}

bool Matter::slotSetfkDefaultParalegal(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkDefaultParalegal", newfk);
    if(result) mDat.fkDefaultParalegal = newfk;
    return result;
}

bool Matter::slotSetfkKeyDocument(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkKeyDocument", newfk);
    if(result) mDat.fkKeyDocument = newfk;
    return result;
}

bool Matter::slotSetfkMatterJurisdiction(Key newfk)
{
    bool result = WriteKey(matterTableName, "fkMatterJurisdiction", newfk);
    if(result) mDat.fkMatterJurisdiction = newfk;
    return result;
}

bool Matter::slotSetOldId(Key newold)
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

} // namespace Nutmeg
