#include "entity.h"

namespace Nutmeg
{

Entity::Entity() : Nutmeg::Object{} {}

Entity::Entity(Key id)
    : Nutmeg::Object(id)
{
    Entity* cachedEntity = cache<Entity>::getObjectFromCache(id, &Entity::GetEntity);
    if (cachedEntity) {
        // If we find the Entity in cache, copy its state
        *this = *cachedEntity;
        mObjectIsNull = false; // Assuming this is inherited from Object
    } else {
        // If not in cache, proceed with initialization
        InitializeEntity(id);
    }
}

// Static method to fetch an Entity from the database if not in cache
Entity* Entity::GetEntity(Key id) {
    Entity* entity = new Entity(id); // This will call the constructor again, but now with cache check
    if (entity->mObjectIsNull) { // Assuming this is inherited from Object
        delete entity; // Clean up if initialization failed
        return nullptr;
    }
    return entity;
}
Entity::Entity(String entityName) : Nutmeg::Object()
{
    Key id = Nutdb::InsertEntity(entityName);
    InitializeEntity(id);
}

bool Entity::slotUpdate(EntityData dat)
{
    bool result = Nutdb::UpdateEntity(dat);
    if(!result) return false;

    result = Object::Commit();
    if(!result) return false;

    return InitializeEntity(dat.EntityId);
}

bool Entity::SetId(Key newval)
{
    return InitializeEntity(newval);
}

bool Entity::Commit()
{
    return slotUpdate(mDat);
}

bool Entity::slotSetEntityName(String newval)
{
    bool result = WriteString(entityTableName, "EntityName", newval);
    if(result) mDat.EntityName = newval;
    return result;
}

bool Entity::slotSetFullLegalName(String newval)
{
    bool result = WriteString(entityTableName, "FullLegalName", newval);
    if(result) mDat.FullLegalName = newval;
    return result;
}

bool Entity::slotSetPrimaryAddress(String newval)
{
    bool result = WriteString(entityTableName, "PrimaryAddress", newval);
    if(result) mDat.PrimaryAddress = newval;
    return result;
}

bool Entity::slotSetSecondaryAddress(String newval)
{
    bool result = WriteString(entityTableName, "SecondaryAddress", newval);
    if(result) mDat.SecondaryAddress = newval;
    return result;
}

bool Entity::slotSetWebsite(String newval)
{
    bool result = WriteString(entityTableName, "Website", newval);
    if(result) mDat.Website = newval;
    return result;
}

bool Entity::slotSetPrimaryPhone(String newval)
{
    bool result = WriteString(entityTableName, "PrimaryPhone", newval);
    if(result) mDat.PrimaryPhone = newval;
    return result;
}

bool Entity::slotSetSecondaryPhone(String newval)
{
    bool result = WriteString(entityTableName, "SecondaryPhone", newval);
    if(result) mDat.SecondaryPhone = newval;
    return result;
}

bool Entity::slotSetFaxNumber(String newval)
{
    bool result = WriteString(entityTableName, "FaxNumber", newval);
    if(result) mDat.FaxNumber = newval;
    return result;
}

bool Entity::slotSetPrimaryEmail(String newval)
{
    bool result = WriteString(entityTableName, "PrimaryEmail", newval);
    if(result) mDat.PrimaryEmail = newval;
    return result;
}

bool Entity::slotSetSecondaryEmail(String newval)
{
    bool result = WriteString(entityTableName, "SecondaryEmail", newval);
    if(result) mDat.SecondaryEmail = newval;
    return result;
}

bool Entity::slotSetfkJurisdiction(Key newval)
{
    bool result = WriteKey(entityTableName, "fkJurisdiction", newval);
    if(result) mDat.fkJurisdiction = newval;
    return result;
}

bool Entity::slotSetDocketPrefix(String newval)
{
    bool result = WriteString(entityTableName, "DocketPrefix", newval);
    if(result) mDat.DocketPrefix = newval;
    return result;
}

bool Entity::slotAssignRole(Key roleid)
{
    bool result = Nutdb::AddRoleToEntity(mDat.EntityId, roleid);
    return result;
}

bool Entity::InitializeEntity(Key id)
{
    if(id == 0)
    {
        mObjectIsNull = true;
        mDat = EntityData();
        InitializeObject(0);
        return false;
    }
    mDat = Nutdb::GetEntity(id);
    if(mDat.EntityId == 0)
        return false;
    return Nutmeg::Object::InitializeObject(id);
}

} // namespace Nutmeg
