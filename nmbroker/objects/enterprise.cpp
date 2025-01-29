#include "enterprise.h"

namespace Nutmeg
{

Enterprise::Enterprise() : Nutmeg::Entity{} {}

Enterprise::Enterprise(Key id)
    : Nutmeg::Entity{id}
{
    auto& enterpriseCache = getCache<Enterprise>();
    if (enterpriseCache.contains(id)) {  // Check if Enterprise is already in cache
        Enterprise* cachedEnterprise = *enterpriseCache.object(id);
        if (cachedEnterprise) {
            *this = *cachedEnterprise;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, proceed with initialization
    InitializeEnterprise(id);
}

Enterprise* Enterprise::GetEnterprise(Key id) {
    auto& enterpriseCache = getCache<Enterprise>();
    if (enterpriseCache.contains(id)) {
        return *enterpriseCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new Enterprise and initialize it
    Enterprise* newEnterprise = new Enterprise(id);
    if (!newEnterprise->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        enterpriseCache.insert(id, &newEnterprise);
        return newEnterprise;
    } else {
        delete newEnterprise;  // Clean up if initialization failed
        return nullptr;
    }
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
