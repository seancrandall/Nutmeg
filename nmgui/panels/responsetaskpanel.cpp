#include "responsetaskpanel.h"
#include "widgets/eibutton.h"

namespace Nutmeg
{

ResponseTaskPanel::ResponseTaskPanel(std::shared_ptr<Response> rsp, QWidget *parent)
    : Nutmeg::TaskPanel(new EIButton(rsp), rsp, parent)
    , response(rsp)
{
}

void Nutmeg::ResponseTaskPanel::slotExtraButtonAction()
{
    AddExaminerInterviewDialog diag(response->ResponseId);
    diag.setModal(true);
    diag.exec();

    return;
}

} // namespace Nutmeg
