#include "task.h"
#include "matter.h"

namespace Nutmeg
{

Task::Task() : Object() {}

Task::Task(Key newid) : Object(newid)
{
    auto& taskCache = getCache<Task>();
    if (taskCache.contains(newid)) {  // Check if Task is already in cache
        Task* cachedTask = *taskCache.object(newid);
        if (cachedTask) {
            *this = *cachedTask;  // Copy state if found in cache
            mObjectIsNull = false;
            return;  // Exit constructor early if we've copied from cache
        }
    }
    // If not in cache, proceed with initialization
    InitializeTask(newid);
}

//bool Task::getNeedsExaminerInterview() { return bNeedsExaminerInterview; }

const QString Task::getTaskTypeString()
{
    return TaskNames[mDat.fkTaskType];
}

Task* Task::GetTask(Key id) {
    auto& taskCache = getCache<Task>();
    if (taskCache.contains(id)) {
        return *taskCache.object(id);  // Return from cache if available
    }

    // If not in cache, create new Task and initialize it
    Task* newTask = new Task(id);
    if (!newTask->mObjectIsNull) { // Assuming mObjectIsNull becomes false on successful initialization
        taskCache.insert(id, &newTask);
        return newTask;
    } else {
        delete newTask;  // Clean up if initialization failed
        return nullptr;
    }
}

bool Task::InsertWithMatter(Key matterid)
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

bool Task::InsertWithMatter(Key matterid, Date triggerDate)
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

bool Task::Update(TaskData dat)
{
    bool result = Nutdb::UpdateTask(dat);
    if(!result) return false;

    result = Object::Commit();
    if(!result) return false;

    return InitializeTask(dat.TaskId);
}

bool Task::SetId(Key newid)
{
    return InitializeTask(newid);
}

bool Task::Commit()
{
    return Update(mDat);
}

bool Task::SetfkMatter(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkMatter", newfk);
    if(result)
        mDat.fkMatter = newfk;
    return result;
}

bool Task::SetDateAssigned(Date newdate)
{
    bool result = WriteDate(taskTableName, "DateAssigned", newdate);
    if (result)
        mDat.DateAssigned = newdate;
    return result;
}

bool Task::SetfkDeadline(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkDeadline", newfk);
    if(result)
        mDat.fkDeadline = newfk;
    return result;
}

bool Task::SetExpectedFee(float newfee)
{
    bool result = WriteValue(taskTableName, "ExpectedFee", QVariant::fromValue(newfee));
    if(result)
        mDat.ExpectedFee = newfee;
    return result;
}

bool Task::SetFeePercent(float newpercent)
{
    bool result = WriteValue(taskTableName, "FeePercent", QVariant::fromValue(newpercent));
    if(result)
        mDat.FeePercent = newpercent;
    return result;
}

bool Task::SetApprovalRequested(Date newdate)
{
    bool result = WriteDate(taskTableName, "ApprovalRequested", newdate);
    if(result)
        mDat.ApprovalRequested = newdate;
    return result;
}

bool Task::SetApprovalReceived(Date newdate)
{
    bool result = WriteDate(taskTableName, "ApprovalReceived", newdate);
    if(result)
        mDat.ApprovalReceived = newdate;
    return result;
}

bool Task::SetComplete(bool newval)
{
    bool result = WriteBoolean(taskTableName, "Complete", newval);
    if(result)
        mDat.Complete = newval;
    return result;
}

bool Task::SetfkWorkAttorney(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkWorkAttorney", newfk);
    if(result)
        mDat.fkWorkAttorney = newfk;
    return result;
}

bool Task::SetfkParalegal(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkParalegal", newfk);
    if(result)
        mDat.fkParalegal = newfk;
    return result;

}

bool Task::SetfkAuthorizationDocument(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkAuthorizationDocument", newfk);
    if(result)
        mDat.fkAuthorizationDocument = newfk;
    return result;
}

bool Task::SetOldTaskId(Key newfk)
{
    bool result = WriteKey(taskTableName, "OldTaskId", newfk);
    if(result)
        mDat.OldTaskId = newfk;
    return result;
}

bool Task::SetfkTaskType(Key newfk)
{
    bool result = WriteKey(taskTableName, "fkTaskType", newfk);
    if(result)
        mDat.fkTaskType = newfk;
    return result;
}



// bool Task::SetExaminerInterviewScheduled(bool newval)
// {
//     // bool result = SetFlagValue("ExaminerInterviewScheduled", newval);
//     // if(result)
//     //     bExaminerInterviewScheduled = newval;
//     // return result;
// }

// bool Task::SetWithParalegal(bool newval)
// {
//     bool result = SetFlagValue("WithParalegal", newval);
//     if(result)
//         bWithParalegal = newval;
//     return result;
// }

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
    // bNeedsExaminerInterview = Nutdb::GetFlag(initid, "NeedsExaminerInterview");
    // bExaminerInterviewScheduled = Nutdb::GetFlag(initid, "ExaminerInterviewScheduled");
    // bWithParalegal = Nutdb::GetFlag(initid, "WithParalegal");

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

bool Task::FindNeedsExaminerInterview()
{
    switch (fkTaskType)
    {
    case 1:
    case 2:
    case 4:
    case 14:
    case 48:
    {
        flag["NeedsExaminerInterview"] = true;
        return true;
        break; //unnecessary
    }
    default:
    {
        flag["NeedsExaminerInterview"] = false;
    }
    }

    return false;
}

// bool Nutmeg::Task::getExaminerInterviewScheduled()
// {
//     return flag["ExaminerInterviewScheduled"];
//     //return bExaminerInterviewScheduled;
// }

// bool Task::getWithParalegal()
// {
//     return flag["WithParalegal"];
// }

} // namespace Nutmeg
