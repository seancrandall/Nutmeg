#ifndef NUTMEG_FILING_H
#define NUTMEG_FILING_H

#include "task.h"

#define filingTableName  "filing"
namespace Nutmeg
{

class Filing : public Nutmeg::Task
{
  public:
    explicit Filing();
      explicit Filing(Key id);

    // Properties
    Property(getId, SetId) Key FilingId;
    Property(getfkFilingStatus, setfkFilingStatus) Key fkFilingStatus;
    Property(getfkAsFiledDocument, setfkAsFiledDocument) Key fkAsFiledDocument;

    // Getters
    virtual Key getId(void) const override { return mDat.FilingId; }
    Key getfkFilingStatus(void) { return mDat.fkFilingStatus; }
    Key getfkAsFiledDocument(void) { return mDat.fkAsFiledDocument; }

    bool slotUpdate(FilingData dat);
    virtual bool SetId(Key newval) override;
    virtual bool Commit(void) override;

    bool setfkFilingStatus(Key newval);
    bool setfkAsFiledDocument(Key newval);

    void holdfkFilingStatus(Key newval){mDat.fkFilingStatus = newval; dirty["fkFilingStatus"] = true;}
    void holdfkAsFiledDocument(Key newval){mDat.fkAsFiledDocument = newval; dirty["fkAsFiledDocument"] = true;}


  protected:
    FilingData mDat;
    bool InitializeFiling(Key id);
private:
    static Filing* GetFiling(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_FILING_H
