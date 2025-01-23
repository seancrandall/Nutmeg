#include "eibutton.h"

namespace Nutmeg
{

EIButton::EIButton(Response *resp, QWidget *parent) : PushButton(parent)
{
    mResponse = resp;

    QPushButton::setText("Schedule EI");

    if (!mResponse->NeedsExaminerInterview)
        hide();

    if (mResponse->ExaminerInterviewScheduled)
        hide();
}

void EIButton::slotHandleClick() {}

} // namespace Nutmeg
