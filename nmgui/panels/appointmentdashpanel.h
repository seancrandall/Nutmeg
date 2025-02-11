#ifndef APPOINTMENTDASHPANEL_H
#define APPOINTMENTDASHPANEL_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "frame.h"
#include "nutmeg.h"
#include "objects/appointment.h"
#include "objects/matter.h"
#include "objects/task.h"
#include "appointmenttaskpanel.h"
#include "panels/flagspanel.h"
#include "widgets/donebutton.h"

namespace Nutmeg {

class AppointmentDashPanel : public Frame
{
    Q_OBJECT
public:
    explicit AppointmentDashPanel(QWidget *parent=nullptr);
    explicit AppointmentDashPanel(Key appointmentId, QWidget *parent=nullptr);
    explicit AppointmentDashPanel(std::shared_ptr<Appointment> pAppt, QWidget *parent=nullptr);

protected:
    QLabel *lAppointmentType;
    PushButton *bDone;

    //Examiner Information
    PushButton *bExaminerName;
    LineEdit *cExaminerPhoneNumber;
    LineEdit *cExaminerEmail;
    PushButton *bEmailExaminer;
    LabeledWidgetLeft *lExaminerPhoneNumber
                     ,*lExaminerEmail;

    AppointmentTaskPanel *pTask;
    FlagsPanel *pFlags;

protected slots:
    void emailExaminer(void);
    void openExaminer(void);
    void markDone(void);

private:
    std::shared_ptr<Appointment> mAppointment;
    std::shared_ptr<Task> mTask;
    std::shared_ptr<Person> mInterviewee;

    void InitializeControls(void);
    void LayoutWidgets(void);
    void ConnectSignalsAndSlots(void);
    void Initialize();
};

} // namespace Nutmeg

#endif // APPOINTMENTDASHPANEL_H
