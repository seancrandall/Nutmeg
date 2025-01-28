#include "responsetaskpanel.h"
#include "widgets/eibutton.h"

namespace Nutmeg
{

ResponseTaskPanel::ResponseTaskPanel(std::shared_ptr<Response> response, QWidget *parent)
    : Nutmeg::TaskPanel(new EIButton(response), response, parent)
    , mResponse(response)
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
