#include "appointmenttaskpanel.h"
#include "objects/patentmatter.h"

namespace Nutmeg {

AppointmentTaskPanel::AppointmentTaskPanel(Key apptId, QWidget *parent)
    : AbstractTaskPanel(apptId, parent)
    , mAppointment(std::make_shared<Appointment>(apptId))
    , mTask(std::make_shared<Task>(mAppointment->associatedObject))
{
    PatentMatter pMatter(mTask->fkMatter);
    bSerialNumber = new DisabledButton(pMatter.ApplicationSerialNumber);
    extraButton = bSerialNumber;
}

void AppointmentTaskPanel::slotExtraButtonAction()
{
    //Button is disabled, so do nothing
}

} // namespace Nutmeg
