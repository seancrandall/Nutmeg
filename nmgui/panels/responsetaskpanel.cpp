#include "responsetaskpanel.h"
#include "widgets/eibutton.h"

namespace Nutmeg
{

ResponseTaskPanel::ResponseTaskPanel(std::shared_ptr<Response> response, QWidget *parent)
    : Nutmeg::AbstractTaskPanel(new EIButton(response), response, parent)
    , mResponse(response)
{
}

ResponseTaskPanel::ResponseTaskPanel(const responsesDashboardEntry &entry, QWidget *parent)
    : ResponseTaskPanel(std::make_shared<Response>(entry.getTaskId()), parent)
{
}

void Nutmeg::ResponseTaskPanel::slotExtraButtonAction()
{
    AddExaminerInterviewDialog diag(mResponse->ResponseId);
    diag.setModal(true);
    diag.exec();

    return;
}

} // namespace Nutmeg
