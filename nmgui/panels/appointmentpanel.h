#ifndef APPOINTMENTPANEL_H
#define APPOINTMENTPANEL_H

#include <QObject>
#include <QWidget>

#include "objects/appointment.h"

#include "widgets/linedisplayid.h"
#include "widgets/datetimeedit.h"

#include "frame.h"

namespace Nutmeg {

class AppointmentPanel : public Nutmeg::Frame
{
    Q_OBJECT
public:
    AppointmentPanel(std::shared_ptr<Appointment> appt, QWidget *parent = nullptr);
    AppointmentPanel(Key apptid, QWidget *parent = nullptr);


protected:
    LineDisplayId *cId;

private:
    std::shared_ptr<Appointment> mAppointment;
};

} // namespace Nutmeg

#endif // APPOINTMENTPANEL_H
