#include "eibutton.h"

namespace Nutmeg
{

EIButton::EIButton(std::shared_ptr<Response> response, QWidget *parent)
    : PushButton(parent)
    , mResponse(response)
{
    QPushButton::setText("Schedule EI");

    if (!mResponse->NeedsExaminerInterview)
        hide();

    if (mResponse->flag["ExaminerInterviewScheduled"])
        hide();
}

void EIButton::slotHandleClick() {}

} // namespace Nutmeg
