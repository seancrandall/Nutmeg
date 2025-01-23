#include "person.h"

namespace Nutmeg
{

Person::Person(QObject *parent)
    : Nutmeg::Entity(parent)
{
    InitializePerson(0);
}

Person::Person(Key id, QObject *parent)
    : Nutmeg::Entity(id, parent)
{
    InitializePerson(id);
}

Person::Person(String first, String last, QObject *parent)
    : Nutmeg::Entity(parent)
{
    Key id = Nutdb::InsertPerson(first, last);
    InitializePerson(id);
}

bool Person::slotSetId(Key newval)
{
    return InitializePerson(newval);
}

bool Person::slotUpdate(PersonData dat)
{
    bool result = Nutdb::UpdatePerson(dat);
    if(!result) return false;

    result = Entity::slotCommit();
    if(!result) return false;

    return InitializePerson(dat.PersonId);
}

bool Person::slotCommit()
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
