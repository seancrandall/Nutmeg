#include "taskdialog.h"

namespace Nutmeg {

TaskDialog::TaskDialog(std::shared_ptr<Task> task, QWidget *parent)
    : Dialog(parent)
    , mTask(task)
    , mMatter(std::make_shared<Matter>(task->fkMatter))
{
    Initialize();
}

TaskDialog::TaskDialog(Key taskid, QWidget *parent)
    : Dialog(parent)
    , mTask(std::make_shared<Task>(taskid))
    , mMatter(std::make_shared<Matter>(mTask->fkMatter))
{
    Initialize();
}

void TaskDialog::slotScatter()
{
    taskPanel->Scatter();
    matterPanel->scatter();
}

void TaskDialog::slotGather()
{
    taskPanel->Gather();
    matterPanel->gather();
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

// void TaskDialog::connectSignalsAndSlots()
// {
//     Dialog::connectSignalsAndSlots();
// }

void TaskDialog::Initialize()
{
    taskPanel = new TaskPanelFull(mTask, this);
    matterPanel = new MatterPanel(mMatter, this);

    setupDisplay();
    connectSignalsAndSlots();
    slotScatter();
}

} // namespace Nutmeg
