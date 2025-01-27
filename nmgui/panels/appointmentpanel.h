#ifndef APPOINTMENTPANEL_H
#define APPOINTMENTPANEL_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>

#include "objects/appointment.h"
#include "dbaccess/appointmenttypemodel.h"

#include "nutmeg.h"
#include "property.h"

#include "widgets/linedisplayid.h"
#include "widgets/datetimeedit.h"
#include "widgets/combobox.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/plusbutton.h"

#include "frame.h"

namespace Nutmeg {

class AppointmentPanel : public Nutmeg::Frame
{
    Q_OBJECT
public:
    AppointmentPanel(std::shared_ptr<Appointment> appt, QWidget *parent = nullptr);
    AppointmentPanel(Key apptid, QWidget *parent = nullptr);
    AppointmentPanel(QWidget *parent = nullptr);

    Property(getAppointment, setAppointment) QDateTime appointment;
    Property(getValid, setValid) bool valid;
    Property(getAppointmentObject, setAppointmentObject) std::shared_ptr<Appointment> appointmentObject;

    QDateTime getAppointment(void);
    void setAppointment(QDateTime newAppointment);
    bool getValid(void);
    void setValid(bool newval);
    void setAppointmentObject(std::shared_ptr<Appointment> newobject);
    std::shared_ptr<Appointment> getAppointmentObject(void);

    void validate(void);
    void invalidate(void);

    void Scatter(void);
    void Gather(void);

signals:
    void signalAppointmentCreated(Key appointmentId);

protected:
    QGridLayout *grid;

    LineDisplayId *cId;
    DateTimeEdit *cDateTime;
    ComboBox *cAppointmentType;
    PlusButton *plus;

    LabeledWidgetLeft *lId;
    LabeledWidgetLeft *lDateTime;
    LabeledWidgetLeft *lAppointmentType;

private:
    std::shared_ptr<Appointment> mAppointment;
    appointmentTypeModel *typeModel;

    void Initialize(void);
    bool mIsValid;

    void ConnectSignalsAndSlots(void);

private slots:
    void slotCreateNewAppointment(void);
};

} // namespace Nutmeg

#endif // APPOINTMENTPANEL_H
