#include "taskpanelfull.h"

namespace Nutmeg {

TaskPanelFull::TaskPanelFull(Task* task, QWidget *parent)
    : Frame(parent)
    , mTask(task)
    , mMatter(new Matter(mTask->fkMatter, this))
{
    Initialize();
}

TaskPanelFull::TaskPanelFull(Key taskid, QWidget *parent)
    : Frame(parent)
    , mTask(new Task(taskid, this))
    , mMatter(new Matter(mTask->fkMatter, this))
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
    cTaskType = new TaskTypeCombo(mTask);
    lTaskType = new LabeledWidgetLeft("Task Type", cTaskType);
    mDeadline = std::make_shared<Deadline>(mTask->fkDeadline);
    cDeadlines = new DeadlinesPanel(mDeadline, false);

    grid = new QGridLayout(this);
    grid->addLayout(lDateAssigned, 0, 0);
    grid->addLayout(lComplete, 0, 1);
    grid->addWidget(cDeadlines, 1, 0, 3, 1);
    grid->addLayout(lTaskType, 1, 1);
    grid->addLayout(lWorkAttorney, 2, 1);
    grid->addLayout(lParalegal, 3, 1);
    grid->addLayout(lExpectedFee, 4, 0);
    grid->addLayout(lFeePercent, 4, 1);
}

} // namespace Nutmeg
