#ifndef APPOINTMENTTASKPANEL_H
#define APPOINTMENTTASKPANEL_H

#include "panels/abstracttaskpanel.h"
#include "objects/appointment.h"
#include "nutmeg.h"
#include "property.h"

namespace Nutmeg {

class AppointmentTaskPanel : public Nutmeg::AbstractTaskPanel
{
    Q_OBJECT
public:
    AppointmentTaskPanel(Key apptId, QWidget *parent=nullptr);

    std::shared_ptr<Appointment> mAppointment;
    std::shared_ptr<Task> mTask;

    DisabledButton *bSerialNumber;

protected:
    virtual void slotExtraButtonAction(void) override;
};

} // namespace Nutmeg

#endif // APPOINTMENTTASKPANEL_H
