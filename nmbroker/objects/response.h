#ifndef NUTMEG_RESPONSE_H
#define NUTMEG_RESPONSE_H

#include "task.h"

#define responseTableName "response"
namespace Nutmeg
{

class Response : public Nutmeg::Task
{
  public:
    explicit Response();
      explicit Response(Key id);
    explicit Response(Key matterId, Date triggerDate);

    // Properties
    Property(getId, slotSetId) Key ResponseId;
    Property(getfkClientOfficeHours, slotSetfkClientOfficeHours) Key fkClientOfficeHours;
    Property(getfkExaminerInterview, slotSetfkExaminerInterview) Key fkExaminerInterview;
    Property(getMailingDate, slotSetMailingDate) Date MailingDate;
    Property(getDateFiled, slotSetDateFiled) Date DateFiled;
    Property(getfkResponseAsFiled, slotSetfkResponseAsFiled) Key fkResponseAsFiled;
    Property(getfkActionDocument, slotSetfkActionDocument) Key fkActionDocument;

    // Getters
    virtual Key getId(void) override { return mDat.ResponseId; }
    Key getfkClientOfficeHours(void) { return mDat.fkClientOfficeHours; }
    Key getfkExaminerInterview(void) { return mDat.fkExaminerInterview; }
    Date getMailingDate(void) { return mDat.MailingDate; }
    Date getDateFiled(void) { return mDat.DateFiled; }
    Key getfkResponseAsFiled(void) { return mDat.fkResponseAsFiled; }
    Key getfkActionDocument(void) { return mDat.fkActionDocument; }

    //Hold properties
    void holdfkClientOfficeHours(Key newval) {mDat.fkClientOfficeHours = newval; dirty["fkClientOfficeHours"] = true;}
    void holdfkExaminerInterview(Key newval) {mDat.fkExaminerInterview = newval; dirty["fkExaminerInterview"] = true;}
    void holdMailingDate(Date newval) {mDat.MailingDate = newval; dirty["MailingDate"] = true;}
    void holdDateFiled(Date newval) {mDat.DateFiled = newval; dirty["DateFiled"] = true;}
    void holdfkREsponseAsFiled(Key newval) {mDat.fkResponseAsFiled = newval; dirty["fkResponseAsFiled"] = true;}
    void holdfkActionDocument(Key newval) {mDat.fkActionDocument = newval; dirty["fkActionDocument"] = true;}

    bool slotUpdate(ResponseData dat);
    virtual bool SetId(Key id) override;
    virtual bool Commit(void) override;

    bool slotSetfkClientOfficeHours(Key newval);
    bool slotSetfkExaminerInterview(Key newval);
    bool slotSetMailingDate(Date newval);
    bool slotSetDateFiled(Date newval);
    bool slotSetfkResponseAsFiled(Key newval);
    bool slotSetfkActionDocument(Key newval);

  protected:
    ResponseData mDat;

    bool InitializeResponse(Key id);
private:
    static Response* GetResponse(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSE_H
