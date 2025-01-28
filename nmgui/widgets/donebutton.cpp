#include "donebutton.h"

namespace Nutmeg
{

DoneButton::DoneButton(Task *task, QWidget *parent)
    : Nutmeg::PushButton(parent)
    , mTask(task)
{
    if (task->WithParalegal == false)
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
    if (mTask->WithParalegal == false)
    {
        mTask->WithParalegal = true;
        setText("Done");
    }
    else
    {
        mTask->Complete = true;
        hide();
    }
}

} // namespace Nutmeg
