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
    Person* cachedPerson = cache<Person>::getObjectFromCache(id, &Person::GetPerson);
    if (cachedPerson) {
        // If we find the Person in cache, copy its state
        *this = *cachedPerson;
        mObjectIsNull = false; // Assuming this is inherited from Object via Entity
    } else {
        // If not in cache, proceed with initialization
        InitializePerson(id);
    }
}

// Static method to fetch a Person from the database if not in cache
Person* Person::GetPerson(Key id) {
    Person* person = new Person(id); // This will call the constructor again, but now with cache check
    if (person->mObjectIsNull) { // Assuming this is inherited from Object via Entity
        delete person; // Clean up if initialization failed
        return nullptr;
    }
    return person;
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
