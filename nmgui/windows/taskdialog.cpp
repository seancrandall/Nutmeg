#include "taskdialog.h"

namespace Nutmeg {

TaskDialog::TaskDialog(Task *task, QWidget *parent)
    : Dialog(parent)
    , mTask(task)
    , mMatter(new Matter(task->fkMatter, this))
{
    Initialize();
}

TaskDialog::TaskDialog(Key taskid, QWidget *parent)
    : Dialog(parent)
    , mTask(new Task(taskid, this))
    , mMatter(new Matter(mTask->fkMatter, this))
{
    Initialize();
}

void TaskDialog::slotScatter()
{

}

void TaskDialog::slotGather()
{

}

void TaskDialog::slotHandleRejected()
{
    Dialog::slotHandleRejected();
}

void TaskDialog::slotHandleAccepted()
{
    Dialog::slotHandleAccepted();
}

void TaskDialog::setupDisplay()
{
    grid->addWidget(taskPanel);
    grid->addWidget(matterPanel);

    workLayout->addLayout(grid, 0, 0);
    //Last Thing
    updateLayout();
}

void TaskDialog::connectSignalsAndSlots()
{
    Dialog::connectSignalsAndSlots();
}

void TaskDialog::Initialize()
{
    taskPanel = new TaskPanelFull(mTask, this);
    matterPanel = new MatterPanel(mMatter, this);

    setupDisplay();
}

} // namespace Nutmeg
