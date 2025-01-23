#ifndef NUTMEG_PERSON_H
#define NUTMEG_PERSON_H

#include "entity.h"
#include <QObject>

namespace Nutmeg
{

class Person : public Nutmeg::Entity
{
    Q_OBJECT
  public:
    explicit Person(QObject *parent = nullptr);
    explicit Person(Key id, QObject *parent = nullptr);
    explicit Person(String first, String last, QObject *parent = nullptr);

    // Properties
    Property(getId, slotSetId) Key PersonId;
    Property(getFirstName, slotSetFirstName) String FirstName;
    Property(getLastName, slotSetLastName) String LastName;
    Property(getfkResidence, slotSetfkResidence) Key fkResidence;
    Property(getfkCitizenship, slotSetfkCitizenship) Key fkCitizenship;
    Property(getOldId, slotSetOldId) Key OldId;

    Property(getNewRole, slotAssignRole) Key NewRole;

    // Getters
    virtual Key getId(void) override { return mDat.PersonId; }

    String getFirstName(void) { return mDat.FirstName; }

    String getLastName(void) { return mDat.LastName; }

    Key getfkResidence(void) { return mDat.fkResidence; }

    Key getfkCitizenship(void) { return mDat.fkCitizenship; }

    Key getOldId(void) { return mDat.OldId; }

    Key getNewRole(void) { return 0; } // This is a write-only property

  public slots:
    virtual bool slotSetId(Key newval) override;
    bool slotUpdate(PersonData dat);
    virtual bool slotCommit(void) override;

    bool slotSetFirstName(String newval);
    bool slotSetLastName(String newval);
    bool slotSetfkResidence(Key newval);
    bool slotSetfkCitizenship(Key newval);
    bool slotSetOldId(Key newval);
    virtual bool slotAssignRole(Key roleid) override;

    void holdFirstName(String newval) {mDat.FirstName = newval; dirty["FirstName"] = true;}
    void holdLastName(String newval) {mDat.LastName = newval; dirty["LastName"] = true;}
    void holdfkResidence(Key newval) {mDat.fkResidence = newval; dirty["fkResidence"] = true;}
    void holdfkCitizenship(Key newval) {mDat.fkCitizenship = newval; dirty["fkCitizenship"] = true;}
    void holdOldId(Key newval) {mDat.OldId = newval; dirty["OldId"] = true;}

  protected:
    bool InitializePerson(Key newid);

    PersonData mDat;
    const QString personTableName = "person";
};

} // namespace Nutmeg

#endif // NUTMEG_PERSON_H
