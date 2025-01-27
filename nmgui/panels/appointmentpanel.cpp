#include "appointmentpanel.h"

namespace Nutmeg {


AppointmentPanel::AppointmentPanel(std::shared_ptr<Appointment> appt, QWidget *parent)
    : Frame(parent)
    , mAppointment(appt)
{

}

AppointmentPanel::AppointmentPanel(Key apptid, QWidget *parent)
    : Frame(parent)
    , mAppointment(std::make_shared<Appointment>(apptid))
{

}

} // namespace Nutmeg
