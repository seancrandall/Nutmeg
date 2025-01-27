#include "filingtaskpanel.h"
#include "widgets/emailinventorsbutton.h"

namespace Nutmeg
{

FilingTaskPanel::FilingTaskPanel(Filing* fil, QWidget *parent)
    : Nutmeg::TaskPanel(new EmailInventorsButton(fil), fil, parent)
{
}

void FilingTaskPanel::slotExtraButtonAction() { return; }

} // namespace Nutmeg
