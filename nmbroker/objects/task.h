#ifndef NUTMEG_TASK_H
#define NUTMEG_TASK_H

#include "object.h"

#define taskTableName "task"
namespace Nutmeg
{

#include <QHash>

QHash<Key, QString> TaskNames = {
    {1, "1 - Nofinal Office Action (US)"},
    {2, "2 - AFCP After Final"},
    {3, "Appeal Brief"},
    {4, "Restriction Requirement"},
    {5, "Missing Parts"},
    {6, "Pay Issue Fee"},
    {7, "CON Recommendation"},
    {8, "Notice of Allowance Review (Aurora)"},
    {9, "Foreign OA Instruction"},
    {10, "Corrected Application Papers"},
    {11, "Foreign Claim Set"},
    {12, "Certificate of Correction"},
    {14, "4 - RCE With Amendment"},
    {15, "3 - RCE Without Amendment"},
    {16, "Proposed Examiner's Amendment"},
    {17, "Terminal Disclaimer"},
    {18, "5 - Notice of Appeal"},
    {19, "Nonprovisional Patent Application"},
    {20, "Provisional Patent Application"},
    {21, "Continuation Application"},
    {22, "CIP Application"},
    {23, "Reissue Application"},
    {24, "Claim Chart"},
    {25, "Trademark Application (Current Use)"},
    {26, "Trademark Application (Intent to Use)"},
    {27, "Trademark Application (International)"},
    {28, "Trademark Office Action"},
    {29, "Statement of Use"},
    {30, "File Specimen"},
    {31, "File Disclaimer"},
    {32, "File Copyright"},
    {33, "Litigation Support Task"},
    {34, "Memorandum"},
    {35, "Reply to Examiner's Answer"}
};

class Task : public Nutmeg::Object
{
  public:
    explicit Task();
      explicit Task(Key id);

    Property(getId, slotSetId) Key TaskId;
    Property(getfkMatter, slotSetfkMatter) Key fkMatter;
    Property(getDateAssigned, slotSetDateAssigned) Date DateAssigned;
    Property(getfkDeadline, slotSetfkDeadline) Key fkDeadline;
    Property(getExpectedFee, slotSetExpectedFee) float ExpectedFee;
    Property(getFeePercent, slotSetFeePercent) float FeePercent;
    Property(getApprovalRequested, slotSetApprovalRequested) Date ApprovalRequested;
    Property(getApprovalReceived, slotSetApprovalReceived) Date ApprovalReceived;
    Property(getComplete, slotSetComplete) bool Complete;
    Property(getfkWorkAttorney, slotSetfkWorkAttorney) Key fkWorkAttorney;
    Property(getfkParalegal, slotSetfkParalegal) Key fkParalegal;
    Property(getfkAuthorizationDocument, slotSetfkAuthorizationDocument) Key fkAuthorizationDocument;
    Property(getOldTaskId, slotSetOldTaskId) Key OldTaskId;
    Property(getfkTaskType, slotSetfkTaskType) Key fkTaskType;
    ReadOnlyProperty(getTaskTypeString) QString TaskTypeString;

    // Flag Properties
    Property(getNeedsExaminerInterview, slotSetNeedsExaminerInterview) bool NeedsExaminerInterview;
    Property(getExaminerInterviewScheduled, slotSetExaminerInterviewScheduled) bool ExaminerInterviewScheduled;
    Property(getWithParalegal, slotSetWithParalegal) bool WithParalegal;

    // Getters
    virtual Key getId(void) override { return mDat.TaskId; }

    Key getfkMatter(void) { return mDat.fkMatter; }
    Date getDateAssigned(void) { return mDat.DateAssigned; }
    Key getfkDeadline(void) { return mDat.fkDeadline; }
    float getExpectedFee(void) { return mDat.ExpectedFee; }
    float getFeePercent(void) { return mDat.FeePercent; }
    Date getApprovalRequested(void) { return mDat.ApprovalRequested; }
    Date getApprovalRecevied(void) { return mDat.ApprovalReceived; }
    bool getComplete(void) { return mDat.Complete; }
    Key getfkWorkAttorney(void) { return mDat.fkWorkAttorney; }
    Key getfkParalegal(void) { return mDat.fkParalegal; }
    Key getAuthorizationDocument(void) { return mDat.fkAuthorizationDocument; }
    Key getOldTaskId(void) { return mDat.OldTaskId; }
    Key getfkTaskType(void) { return mDat.fkTaskType; }
    bool getNeedsExaminerInterview(void) { return bNeedsExaminerInterview; }
    bool getExaminerInterviewScheduled(void) { return bExaminerInterviewScheduled; }
    bool getWithParalegal(void) { return bWithParalegal; }
    const QString getTaskTypeString(void);

    //Hold properties
    void holdfkMatter(Key newkey) {mDat.fkMatter = newkey; dirty["fkMatter"]  = true;}
    void holdDateAssigned(Date newdate) {mDat.DateAssigned = newdate;dirty["DateAssigned"]  = true;}
    void holdfkDeadline(Key newkey) {mDat.fkDeadline = newkey; dirty["fkDeadline"] = true;}
    void holdExpectedFee(float newfee) {mDat.ExpectedFee = newfee; dirty["ExpectedFee"] = true;}
    void holdFeePercent(float newpercent) {mDat.FeePercent = newpercent; dirty["FeePercent"] = true;}
    void holdApprovalRequested(Date newdate) {mDat.ApprovalRequested = newdate; dirty["ApprovalRequested"] = true; }
    void holdApprovalRecevied(Date newdate) {mDat.ApprovalReceived = newdate; dirty["ApprovalReceived"] = true;}
    void holdComplete(bool comp) {mDat.Complete = comp; dirty["Complete"] = true;}
    void holdfkWorkAttorney(Key newkey) {mDat.fkWorkAttorney = newkey; dirty["fkWorkAttorney"] = true;}
    void holdfkParalegal(Key newkey) {mDat.fkParalegal = newkey; dirty["fkParalegal"] = true;}
    void holdfkAuthorizationDocument(Key newkey) {mDat.fkAuthorizationDocument = newkey; dirty["fkAuthorizationDocument"] = true;}
    void holdOldTaskId(Key newkey) {mDat.TaskId = newkey; dirty["TaskId"] = true;}
    void holdfkTaskType(Key newkey) {mDat.fkTaskType = newkey; dirty["fkTaskType"] = true;}
    void holdNeedsExaminerInterview(bool newval) {bNeedsExaminerInterview = newval; dirty["bNeedsExaminerInterview"] = true;}
    void holdExaminerInterviewScheduled(bool newval) {bExaminerInterviewScheduled = newval; dirty["bWithParalegal"] = true;}
    void holdWithParalegal(bool newval) {bWithParalegal = newval;}

    bool slotInsertWithMatter(Key matterid);
    bool slotInsertWithMatter(Key matterid, Date triggerDate);

    bool slotUpdate(TaskData dat);
    virtual bool SetId(Key newid) override;
    virtual bool Commit(void) override;

    bool slotSetfkMatter(Key newfk);
    bool slotSetDateAssigned(Date newdate);
    bool slotSetfkDeadline(Key newfk);
    bool slotSetExpectedFee(float newfee);
    bool slotSetFeePercent(float newpercent);
    bool slotSetApprovalRequested(Date newdate);
    bool slotSetApprovalReceived(Date newdate);
    bool slotSetComplete(bool newval);
    bool slotSetfkWorkAttorney(Key newfk);
    bool slotSetfkParalegal(Key newfk);
    bool slotSetfkAuthorizationDocument(Key newfk);
    bool slotSetOldTaskId(Key newfk);
    bool slotSetfkTaskType(Key newfk);
    bool slotSetNeedsExaminerInterview(bool newval);
    bool slotSetExaminerInterviewScheduled(bool newval);
    bool slotSetWithParalegal(bool newval);

  protected:
    bool InitializeTask(Key id);

    TaskData mDat;
    bool bNeedsExaminerInterview;
    bool bExaminerInterviewScheduled;
    bool bWithParalegal;

    void FindNeedsExaminerInterview(void);
private:
    static Task* GetTask(Key id);
};

} // namespace Nutmeg

#endif // NUTMEG_TASK_H
