#ifndef NUTMEG_TASK_H
#define NUTMEG_TASK_H

#include "object.h"

#define taskTableName "task"
namespace Nutmeg
{

#include <QHash>



class Task : public Nutmeg::Object
{
  public:
    explicit Task();
      explicit Task(Key id);

    Property(getId, SetId) Key TaskId;
    Property(getfkMatter, SetfkMatter) Key fkMatter;
    Property(getDateAssigned, SetDateAssigned) Date DateAssigned;
    Property(getfkDeadline, SetfkDeadline) Key fkDeadline;
    Property(getExpectedFee, SetExpectedFee) float ExpectedFee;
    Property(getFeePercent, SetFeePercent) float FeePercent;
    Property(getApprovalRequested, SetApprovalRequested) Date ApprovalRequested;
    Property(getApprovalReceived, SetApprovalReceived) Date ApprovalReceived;
    Property(getComplete, SetComplete) bool Complete;
    Property(getfkWorkAttorney, SetfkWorkAttorney) Key fkWorkAttorney;
    Property(getfkParalegal, SetfkParalegal) Key fkParalegal;
    Property(getfkAuthorizationDocument, SetfkAuthorizationDocument) Key fkAuthorizationDocument;
    Property(getOldTaskId, SetOldTaskId) Key OldTaskId;
    Property(getfkTaskType, SetfkTaskType) Key fkTaskType;
    ReadOnlyProperty(getTaskTypeString) QString TaskTypeString;
    ReadOnlyProperty(FindNeedsExaminerInterview) bool NeedsExaminerInterview;

    // Flag Properties
    // Property(getNeedsExaminerInterview, SetNeedsExaminerInterview) bool NeedsExaminerInterview;
    // Property(getExaminerInterviewScheduled, SetExaminerInterviewScheduled) bool ExaminerInterviewScheduled;
    // Property(getWithParalegal, SetWithParalegal) bool WithParalegal;

    // Getters
    virtual Key getId(void) const override { return mDat.TaskId; }

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
    bool getNeedsExaminerInterview(void);
    bool getExaminerInterviewScheduled(void);
    bool getWithParalegal(void);
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
    //void holdNeedsExaminerInterview(bool newval) {flag["NeedsExaminerInterview"] = newval;}
    //void holdExaminerInterviewScheduled(bool newval) {flag["ExaminerInterviewScheduled"] = newval;}
    //void holdWithParalegal(bool newval) {flag["WithParalegal"] = newval;}

    bool InsertWithMatter(Key matterid);
    bool InsertWithMatter(Key matterid, Date triggerDate);

    bool Update(TaskData dat);
    virtual bool SetId(Key newid) override;
    virtual bool Commit(void) override;

    bool SetfkMatter(Key newfk);
    bool SetDateAssigned(Date newdate);
    bool SetfkDeadline(Key newfk);
    bool SetExpectedFee(float newfee);
    bool SetFeePercent(float newpercent);
    bool SetApprovalRequested(Date newdate);
    bool SetApprovalReceived(Date newdate);
    bool SetComplete(bool newval);
    bool SetfkWorkAttorney(Key newfk);
    bool SetfkParalegal(Key newfk);
    bool SetfkAuthorizationDocument(Key newfk);
    bool SetOldTaskId(Key newfk);
    bool SetfkTaskType(Key newfk);
    bool FindNeedsExaminerInterview(void);
    //bool SetNeedsExaminerInterview(bool newval);
    //bool SetExaminerInterviewScheduled(bool newval);
    //bool SetWithParalegal(bool newval);

  protected:
    bool InitializeTask(Key id);

    TaskData mDat;
    // bool bNeedsExaminerInterview;
    // bool bExaminerInterviewScheduled;
    // bool bWithParalegal;

private:
    static Task* GetTask(Key id);

    QHash<Key, QString> TaskNames = {
        {1, "Nofinal Office Action (US)"},
        {2, "AFCP After Final"},
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
        {14, "RCE With Amendment"},
        {15, "RCE Without Amendment"},
        {16, "Proposed Examiner's Amendment"},
        {17, "Terminal Disclaimer"},
        {18, "Notice of Appeal"},
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
};

} // namespace Nutmeg

#endif // NUTMEG_TASK_H
