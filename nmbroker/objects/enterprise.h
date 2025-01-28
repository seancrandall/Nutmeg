#ifndef NUTMEG_ENTERPRISE_H
#define NUTMEG_ENTERPRISE_H

#include "entity.h"

#define enterpriseTableName "enterprise"
namespace Nutmeg
{

class Enterprise : public Nutmeg::Entity
{
  public:
    explicit Enterprise();
      explicit Enterprise(Key id);
    explicit Enterprise(String enterpriseName);

    // Properties
    Property(getId, slotSetId) Key EnterpriseId;
    Property(getEnterpriseName, slotSetEnterpriseName) String EnterpriseName;
    Property(getfkBusinessJurisdiction, slotSetfkBusinessJurisdiction) Key fkBusinessJurisdiction;
    Property(getfkStateOfIncorporation, slotSetfkStateOfIncorporation) Key fkStateOfIncorporation;
    Property(getfkMainContact, slotSetfkMainContact) Key fkMainContact;
    Property(getOldOrganizationId, slotSetOldOrganizationId) Key OldOrganizationId;

    // Getters
    virtual Key getId(void) override { return mDat.EnterpriseId; }
    String getEnterpriseName(void) { return mDat.EnterpriseName; }
    Key getfkBusinessJurisdiction(void) { return mDat.fkBusinessJurisdiction; }
    Key getfkStateOfIncorporation(void) { return mDat.fkStateOfIncorporation; }
    Key getfkMainContact(void) { return mDat.fkMainContact; }
    Key getOldOrganizationId(void) { return mDat.OldOrganizationId; }

    virtual bool SetId(Key newval) override;
    bool slotUpdate(EnterpriseData dat);
    virtual bool Commit(void) override;

    bool slotSetEnterpriseName(String newval);
    bool slotSetfkBusinessJurisdiction(Key newval);
    bool slotSetfkStateOfIncorporation(Key newval);
    bool slotSetfkMainContact(Key newval);
    bool slotSetOldOrganizationId(Key newval);

    void holdEnterpriseName(String newval) {mDat.EnterpriseName = newval; dirty["EnterpriseName"] = true;}
    void holdfkBusinessJurisdiction(Key newval){mDat.fkBusinessJurisdiction = newval; dirty["fkBusinessJurisdiction"] = true;}
    void holdfkStateOfIncorporation(Key newval){mDat.fkStateOfIncorporation = newval; dirty["fkStateOfIncorporation"] = true;}
    void holdfkMainContact(Key newval){mDat.fkMainContact = newval; dirty["fkMainContact"] = true;}
    void holdOldOrganizationId(Key newval){mDat.OldOrganizationId = newval; dirty["OldOrganizationId"] = true;}

  protected:
    bool InitializeEnterprise(Key id);

    EnterpriseData mDat;
private:
    static Enterprise* GetEnterprise(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_ENTERPRISE_H
