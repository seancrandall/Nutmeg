#include "enterprise.h"

namespace Nutmeg
{

Enterprise::Enterprise(QObject *parent) : Nutmeg::Entity{parent} {}

Enterprise::Enterprise(Key id, QObject *parent) : Nutmeg::Entity{id, parent}
{
    InitializeEnterprise(id);
}

Enterprise::Enterprise(String enterpriseName, QObject *parent) : Nutmeg::Entity{parent}
{
    Key newid = Nutdb::InsertEnterprise(enterpriseName);
    InitializeEnterprise(newid);
}

bool Enterprise::SetId(Key newval)
{
    return InitializeEnterprise(newval);
}

bool Enterprise::slotUpdate(EnterpriseData dat)
{
    bool result = Nutdb::UpdateEnterprise(dat);
    if(!result) return false;

    result = Entity::Commit();
    if(!result) return false;

    return InitializeEnterprise(dat.EnterpriseId);
}

bool Enterprise::Commit()
{
    return slotUpdate(mDat);
}

bool Enterprise::slotSetEnterpriseName(String newval)
{
    bool result = WriteString(enterpriseTableName, "EnterpriseName", newval);
    if(result) mDat.EnterpriseName = newval;
    return result;
}

bool Enterprise::slotSetfkBusinessJurisdiction(Key newval)
{
    bool result = WriteKey(enterpriseTableName, "fkBusinessJurisdiction", newval);
    if(result) mDat.fkBusinessJurisdiction = newval;
    return result;
}

bool Enterprise::slotSetfkStateOfIncorporation(Key newval)
{
    bool result = WriteKey(enterpriseTableName, "fkStateOfIncorporation", newval);
    if(result) mDat.fkStateOfIncorporation = newval;
    return result;
}

bool Enterprise::slotSetfkMainContact(Key newval)
{
    bool result = WriteKey(enterpriseTableName, "fkMainContact", newval);
    if(result) mDat.fkMainContact = newval;
    return result;
}

bool Enterprise::slotSetOldOrganizationId(Key newval)
{
    bool result = WriteKey(enterpriseTableName, "OldOrganizationId", newval);
    if(result) mDat.OldOrganizationId = newval;
    return result;
}

bool Enterprise::InitializeEnterprise(Key id)
{
    if(id == 0)
    {
        mDat = EnterpriseData();
        return InitializeEntity(0);
    }

    mDat = Nutdb::GetEnterprise(id);
    if(mDat.EnterpriseId == 0) return InitializeEnterprise(0);

    return InitializeEntity(id);
}

} // namespace Nutmeg
