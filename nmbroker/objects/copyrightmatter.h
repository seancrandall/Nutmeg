#ifndef NUTMEG_COPYRIGHTMATTER_H
#define NUTMEG_COPYRIGHTMATTER_H

#include "matter.h"

#define copyrightMatterTableName "copyrightMatter"

namespace Nutmeg
{

class CopyrightMatter : public Nutmeg::Matter
{
  public:
    explicit CopyrightMatter();
      explicit CopyrightMatter(Key id);
    explicit CopyrightMatter(String docketNumber);

    // Properties
    Property(getId, slotSetId) Key CopyrightMatterId;
    Property(getfkAuthor, slotSetfkAuthor) Key fkAuthor;
    Property(getCreated, slotSetCreated) Date Created;
    Property(getFiled, slotSetFiled) Date Filed;
    Property(getRegistered, slotSetRegistered) Date Registered;
    Property(getSerialNumber, slotSetSerialNumber) String SerialNumber;
    Property(getRegistrationNumer, slotSetRegistrationNumber) String RegistrationNumber;
    Property(getfkDeposit, slotSetfkDeposit) Key fkDeposit;
    Property(getfkWorkType, slotSetfkWorkType) Key fkWorkType;

    Property(getIsRegistered, slotSetIsRegistered) bool IsRegistered;

    Key AddAuthor(String first, String last);

    // Getters
    virtual Key getId(void) const override { return mDat.CopyrightMatterId; }
    Key getfkAuthor(void) { return mDat.fkAuthor; }
    Date getCreated(void) { return mDat.Created; }
    Date getFiled(void) { return mDat.Filed; }
    Date getRegistered(void) { return mDat.Registered; }
    String getSerialNumber(void) { return mDat.SerialNumber; }
    String getRegistrationNumber(void) { return mDat.RegistrationNumber; }
    Key getfkDeposit(void) { return mDat.fkDeposit; }
    Key getfkWorkType(void) { return mDat.fkWorkType; }

    bool getIsRegistered(void);

    virtual bool SetId(Key newval) override;
    bool slotUpdate(CopyrightMatterData dat);
    virtual bool Commit(void) override;

    bool slotSetfkAuthor(Key newval);
    bool slotSetCreated(Date newval);
    bool slotSetFiled(Date newval);
    bool slotSetRegistered(Date newval);
    bool slotSetSerialNumber(String newval);
    bool slotSetRegistrationNumber(String newval);
    bool slotSetfkDeposit(Key newval);
    bool slotSetfkWorkType(Key newval);
    bool slotSetIsRegistered(bool newval);

    void holdSetfkAuthor(Key newval) {mDat.fkAuthor = newval; dirty["fkAuthor"] = true;}
    void holdSetCreated(Date newval) {mDat.Created = newval; dirty["Created"] = true;}
    void holdSetFiled(Date newval) {mDat.Filed = newval; dirty["Filed"] = true;}
    void holdSetRegistered(Date newval) {mDat.Registered = newval; dirty["Registered"] = true;}
    void holdSetSerialNumber(String newval) {mDat.SerialNumber = newval; dirty["SerialNumber"] = true;}
    void holdSetRegistrationNumber(String newval) {mDat.RegistrationNumber = newval; dirty["RegistrationNumber"] = true;}
    void holdSetfkDeposit(Key newval) {mDat.fkDeposit = newval; dirty["fkDeposit"] = true;}
    void holdSetfkWorkType(Key newval) {mDat.fkWorkType = newval; dirty["fkWorkType"] = true;}
    void holdSetIsRegistered(bool newval) {mIsRegistered = newval; dirty["Registered"] = true;}

  protected:
    bool InitializeCopyrightMatter(Key id);

    CopyrightMatterData mDat;
    bool mIsRegistered = false;
private:
    static CopyrightMatter* GetCopyrightMatter(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_COPYRIGHTMATTER_H
