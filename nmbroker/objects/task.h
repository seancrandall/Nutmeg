#ifndef NUTMEG_TASK_H
#define NUTMEG_TASK_H

#include "object.h"
#include <QObject>

namespace Nutmeg
{

class Task : public Nutmeg::Object
{
    Q_OBJECT
  public:
    explicit Task(QObject *parent = nullptr);
    explicit Task(Key id, QObject *parent = nullptr);

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

  public slots:
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
    const QString taskTableName = "task";
};

} // namespace Nutmeg

#endif // NUTMEG_TASK_H
