#ifndef NUTMEG_ENTITY_H
#define NUTMEG_ENTITY_H

#include "object.h"

#define entityTableName "entity"
namespace Nutmeg
{

class Entity : public Nutmeg::Object
{
  public:
    explicit Entity();
      explicit Entity(Key id);
    explicit Entity(String entityName);

    // Properties
    Property(getId, slotSetId) Key EntityId;
    Property(getEntityName, slotSetEntityName) String EntityName;
    Property(getFullLegalName, slotSetFullLegalName) String FullLegalName;
    Property(getPrimaryAddress, slotSetPrimaryAddress) String PrimaryAddress;
    Property(getSecondaryAddress, slotSetSecondaryAddress) String SecondaryAddress;
    Property(getWebsite, slotSetWebsite) String Website;
    Property(getPrimaryPhone, slotSetPrimaryPhone) String PrimaryPhone;
    Property(getSecondaryPhone, slotSetSecondaryPhone) String SecondaryPhone;
    Property(getFaxNumber, slotSetFaxNumber) String FaxNumber;
    Property(getPrimaryEmail, slotSetPrimaryEmail) String PrimaryEmail;
    Property(getSecondaryEmail, slotSetSecondaryEmail) String SecondaryEmail;
    Property(getfkJurisdiction, slotSetfkJurisdiction) Key fkJurisdiction;
    Property(getDocketPrefix, slotSetDocketPrefix) String DocketPrefix;

    // Getters
    virtual Key getId(void) override { return mDat.EntityId; }
    String getEntityName(void) { return mDat.EntityName; }
    String getFullLegalName(void) { return mDat.FullLegalName; }
    String getPrimaryAddress(void) { return mDat.PrimaryAddress; }
    String getSecondaryAddress(void) { return mDat.SecondaryAddress; }
    String getWebsite(void) { return mDat.Website; }
    String getPrimaryPhone(void) { return mDat.PrimaryPhone; }
    String getSecondaryPhone(void) { return mDat.SecondaryPhone; }
    String getFaxNumber(void) { return mDat.FaxNumber; }
    String getPrimaryEmail(void) { return mDat.PrimaryEmail; }
    String getSecondaryEmail(void) { return mDat.SecondaryEmail; }
    Key getfkJurisdiction(void) { return mDat.fkJurisdiction; }
    String getDocketPrefix(void) { return mDat.DocketPrefix; }

    bool slotUpdate(EntityData dat);
    virtual bool SetId(Key newval) override;
    virtual bool Commit(void) override;

    bool slotSetEntityName(String newval);
    bool slotSetFullLegalName(String newval);
    bool slotSetPrimaryAddress(String newval);
    bool slotSetSecondaryAddress(String newval);
    bool slotSetWebsite(String newval);
    bool slotSetPrimaryPhone(String newval);
    bool slotSetSecondaryPhone(String newval);
    bool slotSetFaxNumber(String newval);
    bool slotSetPrimaryEmail(String newval);
    bool slotSetSecondaryEmail(String newval);
    bool slotSetfkJurisdiction(Key newval);
    bool slotSetDocketPrefix(String newval);

    void holdSetEntityName(String newval){mDat.EntityName = newval; dirty["EntityName"] = true;}
    void holdSetFullLegalName(String newval){mDat.FullLegalName = newval; dirty["FullLegalName"] = true;}
    void holdSetPrimaryAddress(String newval){mDat.PrimaryAddress = newval; dirty["PrimaryAddress"] = true;}
    void holdSetSecondaryAddress(String newval){mDat.SecondaryAddress = newval; dirty["SecondaryAddress"] = true;}
    void holdSetWebsite(String newval){mDat.Website = newval; dirty["Website"] = true;}
    void holdSetPrimaryPhone(String newval){mDat.PrimaryPhone = newval; dirty["PrimaryPhone"] = true;}
    void holdSetSecondaryPhone(String newval){mDat.SecondaryPhone = newval; dirty["SecondaryPhone"] = true;}
    void holdSetFaxNumber(String newval){mDat.FaxNumber = newval; dirty["FaxNumber"] = true;}
    void holdSetPrimaryEmail(String newval){mDat.PrimaryEmail = newval; dirty["PrimaryEmail"] = true;}
    void holdSetSecondaryEmail(String newval){mDat.SecondaryEmail = newval; dirty["SecondaryEmail"] = true;}
    void holdSetfkJurisdiction(Key newval){mDat.fkJurisdiction = newval; dirty["fkJurisdiction"] = true;}
    void holdSetDocketPrefix(String newval){mDat.DocketPrefix = newval; dirty["DocketPrefix"] = true;}

    virtual bool slotAssignRole(Key roleid);

  protected:
    bool InitializeEntity(Key id);

    EntityData mDat;
private:
    static Entity* GetEntity(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_ENTITY_H
