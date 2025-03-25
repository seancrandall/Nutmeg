#include "taskpanelfull.h"

namespace Nutmeg {

TaskPanelFull::TaskPanelFull(std::shared_ptr<Task> task, QWidget *parent)
    : Frame(parent)
    , mTask(task)
    , mMatter(std::make_shared<Matter>(mTask->fkMatter))
{
    Initialize();
}

TaskPanelFull::TaskPanelFull(Key taskid, QWidget *parent)
    : Frame(parent)
    , mTask(std::make_shared<Task>(taskid))
    , mMatter(std::make_shared<Matter>(mTask->fkMatter))
{
    Initialize();
}

void TaskPanelFull::Scatter()
{
    cDateAssigned->date = mTask->DateAssigned;
    cComplete->setChecked(mTask->Complete);
    cExpectedFee->setValue(mTask->ExpectedFee);
    cFeePercent->setValue(mTask->FeePercent);
    cFeePercent->setMaximum(100.00);
    cWorkAttorney->key = mTask->fkWorkAttorney;
    cParalegal->key = mTask->fkParalegal;
    cTaskType->key = mTask->fkTaskType;

    //If the work attorney or paralegal is not set, set them to the default
    if(mTask->fkWorkAttorney == 0)
        if(mMatter->fkDefaultWorkAttorney != 0)
            cWorkAttorney->key = mMatter->fkDefaultWorkAttorney;
    if(mTask->fkParalegal == 0)
        if(mMatter->fkDefaultParalegal != 0)
            cParalegal->key = mMatter->fkDefaultParalegal;

}

void TaskPanelFull::Gather()
{
    cDeadlines->Gather();
    mTask->holdDateAssigned(cDateAssigned->date);
    mTask->holdComplete(cComplete->checkState());
    mTask->holdExpectedFee(cExpectedFee->value());
    mTask->holdFeePercent(cFeePercent->value());
    mTask->holdfkWorkAttorney(cWorkAttorney->key);
    mTask->holdfkParalegal(cParalegal->key);
    mTask->holdfkTaskType(cTaskType->key);
    mTask->commit();
}

void TaskPanelFull::Initialize()
{
    cId = new LineDisplayId(mTask->getId());
    lId = new LabeledWidgetLeft("Task ID", cId);
    cTaskType = new TaskTypeCombo(mTask);
    lTaskType = new LabeledWidgetLeft("Task Type", cTaskType);
    mDeadline = std::make_shared<Deadline>(mTask->fkDeadline);
    cDeadlines = new DeadlinesPanel(mDeadline, false);

    grid = new QGridLayout(this);
    grid->addLayout(lId, 0, 0);
    grid->addLayout(lDateAssigned, 1, 0);
    grid->addLayout(lComplete, 1, 1);
    grid->addWidget(cDeadlines, 2, 0, 3, 1);
    grid->addLayout(lTaskType, 2, 1);
    grid->addLayout(lWorkAttorney, 3, 1);
    grid->addLayout(lParalegal, 4, 1);
    grid->addLayout(lExpectedFee, 5, 0);
    grid->addLayout(lFeePercent, 5, 1);
}

} // namespace Nutmeg
