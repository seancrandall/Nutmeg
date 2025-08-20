#include "donebutton.h"

namespace Nutmeg
{

DoneButton::DoneButton(std::shared_ptr<Task> task, QWidget *parent)
    : Nutmeg::PushButton(parent)
    , mTask(task)
{
    if (task->flag["WithParalegal"] == false)
    {
        setText("Send to\nParalegal");
    }
    else
    {
        setText("Done");
    }
    setMaximumWidth(100);
}

DoneButton::DoneButton(const responsesDashboardEntry &entry, QWidget *parent)
    : Nutmeg::PushButton(parent)
    , mTask(std::make_shared<Task>(entry.getTaskId()))
{
    if (!entry.getWithParalegal())
    {
        setText("Send to\nParalegal");
    }
    else
    {
        setText("Done");
    }
    setMaximumWidth(100);
}

void DoneButton::slotHandleClicked()
{
    if (mTask->flag["WithParalegal"] == false)
    {
        mTask->flag["WithParalegal"] = true;
        setText("Done");
    }
    else
    {
        mTask->Complete = true;
        hide();
    }
}

} // namespace Nutmeg
