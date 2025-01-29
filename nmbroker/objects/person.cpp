#include "person.h"

namespace Nutmeg
{

Person::Person()
    : Nutmeg::Entity()
{
    InitializePerson(0);
}

Person::Person(Key id)
    : Nutmeg::Entity(id)
{
    auto& personCache = getCache<Person>();
    if (personCache.contains(id)) {  // Check if Person is already in cache
        Person* cachedPerson = *personCache.object(id);
        if (cachedPerson) {
            *this = *cachedPerson;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, proceed with initialization
    InitializePerson(id);
}

Person* Person::GetPerson(Key id) {
    auto& personCache = getCache<Person>();
    if (personCache.contains(id)) {
        return *personCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new Person and initialize it
    Person* newPerson = new Person(id);
    if (!newPerson->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        personCache.insert(id, &newPerson);
        return newPerson;
    } else {
        delete newPerson;  // Clean up if initialization failed
        return nullptr;
    }
}

Person::Person(String first, String last)
    : Nutmeg::Entity()
{
    Key id = Nutdb::InsertPerson(first, last);
    InitializePerson(id);
}

bool Person::SetId(Key newval)
{
    return InitializePerson(newval);
}

bool Person::slotUpdate(PersonData dat)
{
    bool result = Nutdb::UpdatePerson(dat);
    if(!result) return false;

    result = Entity::Commit();
    if(!result) return false;

    return InitializePerson(dat.PersonId);
}

bool Person::Commit()
{
    return slotUpdate(mDat);
}

bool Person::slotSetFirstName(String newval)
{
    bool result = WriteString(personTableName, "FirstName", newval);
    if(result) mDat.FirstName = newval;
    return result;
}

bool Person::slotSetLastName(String newval)
{
    bool result = WriteString(personTableName, "LastName", newval);
    if(result) mDat.LastName = newval;
    return result;
}

bool Person::slotSetfkResidence(Key newval)
{
    bool result = WriteKey(personTableName, "fkResidence", newval);
    if(result) mDat.fkResidence = newval;
    return result;
}

bool Person::slotSetfkCitizenship(Key newval)
{
    bool result = WriteKey(personTableName, "fkCitizenship", newval);
    if(result) mDat.fkCitizenship = newval;
    return result;
}

bool Person::slotSetOldId(Key newval)
{
    bool result = WriteKey(personTableName, "OldId", newval);
    if(result) mDat.OldId = newval;
    return result;
}

bool Person::slotAssignRole(Key roleid)
{
   return Nutdb::AddRoleToPerson(mDat.PersonId, roleid);
}

bool Person::InitializePerson(Key newid)
{
    if(newid == 0)
    {
        mDat = PersonData();
        return InitializeEntity(0);
    }

    mDat = Nutdb::GetPerson(newid);
    if(mDat.PersonId == 0) return InitializePerson(0);

    return Entity::InitializeEntity(newid);
}

} // namespace Nutmeg
