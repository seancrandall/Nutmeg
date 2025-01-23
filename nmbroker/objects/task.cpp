#include "task.h"
#include "matter.h"

namespace Nutmeg
{

Task::Task(QObject *parent) : Object(parent) {}

Task::Task(Key newid, QObject *parent) : Object(parent)
{
    InitializeTask(newid);
}

bool Task::slotInsertWithMatter(Key matterid)
{
    Key newid = Nutdb::InsertTask(matterid);
    if(newid)
    {
        mDat.TaskId = newid;
        InitializeTask(mDat.TaskId);
        return true;
    }
    else return false;
}

bool Task::slotInsertWithMatter(Key matterid, Date triggerDate)
{
    Key newid = Nutdb::InsertTask(matterid, triggerDate);
    if(newid)
    {
        mDat.TaskId = newid;
        InitializeTask(mDat.TaskId);
        return true;
    }
    else return false;
}

bool Task::slotUpdate(TaskData dat)
{
    bool result = Nutdb::UpdateTask(dat);
    if(!result) return false;

    result = Object::slotCommit();
    if(!result) return false;

    return InitializeTask(dat.TaskId);
}

bool Task::slotSetId(Key newid)
{
    return InitializeTask(newid);
}

bool Task::slotCommit()
{
    return slotUpdate(mDat);
}

bool Task::slotSetfkMatter(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkMatter", newfk);
    if(result)
        mDat.fkMatter = newfk;
    return result;
}

bool Task::slotSetDateAssigned(Date newdate)
{
    bool result = WriteDate(taskTableName, "DateAssigned", newdate);
    if (result)
        mDat.DateAssigned = newdate;
    return result;
}

bool Task::slotSetfkDeadline(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkDeadline", newfk);
    if(result)
        mDat.fkDeadline = newfk;
    return result;
}

bool Task::slotSetExpectedFee(float newfee)
{
    bool result = WriteValue(taskTableName, "ExpectedFee", QVariant::fromValue(newfee));
    if(result)
        mDat.ExpectedFee = newfee;
    return result;
}

bool Task::slotSetFeePercent(float newpercent)
{
    bool result = WriteValue(taskTableName, "FeePercent", QVariant::fromValue(newpercent));
    if(result)
        mDat.FeePercent = newpercent;
    return result;
}

bool Task::slotSetApprovalRequested(Date newdate)
{
    bool result = WriteDate(taskTableName, "ApprovalRequested", newdate);
    if(result)
        mDat.ApprovalRequested = newdate;
    return result;
}

bool Task::slotSetApprovalReceived(Date newdate)
{
    bool result = WriteDate(taskTableName, "ApprovalReceived", newdate);
    if(result)
        mDat.ApprovalReceived = newdate;
    return result;
}

bool Task::slotSetComplete(bool newval)
{
    bool result = WriteBoolean(taskTableName, "Complete", newval);
    if(result)
        mDat.Complete = newval;
    return result;
}

bool Task::slotSetfkWorkAttorney(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkWorkAttorney", newfk);
    if(result)
        mDat.fkWorkAttorney = newfk;
    return result;
}

bool Task::slotSetfkParalegal(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkParalegal", newfk);
    if(result)
        mDat.fkParalegal = newfk;
    return result;

}

bool Task::slotSetfkAuthorizationDocument(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkAuthorizationDocument", newfk);
    if(result)
        mDat.fkAuthorizationDocument = newfk;
    return result;
}

bool Task::slotSetOldTaskId(Key newfk)
{
    bool result = WriteKey(taskTableName, "OldTaskId", newfk);
    if(result)
        mDat.OldTaskId = newfk;
    return result;
}

bool Task::slotSetfkTaskType(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkTaskType", newfk);
    if(result)
        mDat.fkTaskType = newfk;
    return result;
}

bool Task::slotSetNeedsExaminerInterview(bool newval)
{
    bool result = SetFlagValue("NeedsExaminerInterview", newval);
    if(result)
        bNeedsExaminerInterview = newval;
    return result;
}

bool Task::slotSetExaminerInterviewScheduled(bool newval)
{
    bool result = SetFlagValue("ExaminerInterviewScheduled", newval);
    if(result)
        bExaminerInterviewScheduled = newval;
    return result;
}

bool Task::slotSetWithParalegal(bool newval)
{
    bool result = SetFlagValue("WithParalegal", newval);
    if(result)
        bWithParalegal = newval;
    return result;
}

bool Task::InitializeTask(Key initid)
{
    if(initid == 0)
    {
        mDat = TaskData();
        return InitializeObject(0);
    }

    mDat = Nutdb::GetTask(initid);

    if(mDat.TaskId == 0)
    {
        return InitializeTask(0);
    }

    // Initialize Flags
    bNeedsExaminerInterview = Nutdb::GetFlag(initid, "NeedsExaminerInterview");
    bExaminerInterviewScheduled = Nutdb::GetFlag(initid, "ExaminerInterviewScheduled");
    bWithParalegal = Nutdb::GetFlag(initid, "WithParalegal");

    //If we don't have an assigned paralegal and work attorney, grab
    //the defaults from the parent case
    Matter matter(fkMatter);
    if(fkParalegal == 0)
    {
        fkParalegal = matter.fkDefaultParalegal;
    }
    if(fkWorkAttorney == 0)
    {
        fkWorkAttorney = matter.fkDefaultWorkAttorney;
    }

    return Object::InitializeObject(mDat.TaskId);
}

void Task::FindNeedsExaminerInterview()
{
    switch (fkTaskType)
    {
    case 1:
    case 2:
    case 4:
    case 14:
    case 48:
    {
        NeedsExaminerInterview = true;
        break;
    }
    default:
    {
        NeedsExaminerInterview = false;
    }
    }
}

} // namespace Nutmeg
