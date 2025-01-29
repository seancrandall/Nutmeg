#include "enterprise.h"

namespace Nutmeg
{

Enterprise::Enterprise() : Nutmeg::Entity{} {}

Enterprise::Enterprise(Key id)
    : Nutmeg::Entity{id}
{
    auto& enterpriseCache = getCache<Enterprise>();
    Enterprise* cachedEnterprise = cache<Enterprise>::getObjectFromCache(id, &Enterprise::GetEnterprise, enterpriseCache);
    if (cachedEnterprise) {
        // If we find the Enterprise in cache, copy its state
        *this = *cachedEnterprise;
        mObjectIsNull = false; // Assuming this is inherited from Object via Entity
    } else {
        // If not in cache, proceed with initialization
        InitializeEnterprise(id);
    }
}

// Static method to fetch an Enterprise from the database if not in cache
Enterprise* Enterprise::GetEnterprise(Key id) {
    Enterprise* enterprise = new Enterprise(id); // This will call the constructor again, but now with cache check
    if (enterprise->mObjectIsNull) { // Assuming this is inherited from Object via Entity
        delete enterprise; // Clean up if initialization failed
        return nullptr;
    }
    return enterprise;
}
Enterprise::Enterprise(String enterpriseName) : Nutmeg::Entity{}
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
