#include "taskpanelfull.h"

namespace Nutmeg {

TaskPanelFull::TaskPanelFull(Task* task, QWidget *parent)
    : Frame(parent)
    , mTask(task)
{
    Initialize();
}

TaskPanelFull::TaskPanelFull(Key taskid, QWidget *parent)
    : Frame(parent)
    , mTask(new Task(taskid, this))
{
    Initialize();
}

void TaskPanelFull::Scatter()
{

}

void TaskPanelFull::Gather()
{

}

void TaskPanelFull::Initialize()
{
    cTaskType = new TaskTypeCombo(mTask);
    lTaskType = new LabeledWidgetLeft("Task Type", cTaskType);
    mDeadline = std::make_shared<Deadline>(mTask->fkDeadline);

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
