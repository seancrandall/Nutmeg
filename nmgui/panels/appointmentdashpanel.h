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
#include "panels/examinerinfopanel.h"
#include "panels/flagspanel.h"
#include "widgets/donebutton.h"
#include "widgets/timeedit.h"
#include "widgets/collapsebutton.h"         // Added for CollapseButton
#include "widgets/docketnumberbutton.h"     // Added for DocketNumberButton
#include "widgets/dynamicstackedwidget.h"   // Added for DynamicStackedWidget

namespace Nutmeg {

class AppointmentDashPanel : public Frame
{
    Q_OBJECT
public:
    explicit AppointmentDashPanel(QWidget *parent = nullptr);
    explicit AppointmentDashPanel(Key appointmentId, QWidget *parent = nullptr);
    explicit AppointmentDashPanel(std::shared_ptr<Appointment> pAppt, QWidget *parent = nullptr);

public slots:
    void expand();    // Added to programmatically expand the panel
    void collapse();  // Added to programmatically collapse the panel

protected:
    std::shared_ptr<Appointment> mAppointment;
    std::shared_ptr<Task> mTask;
    std::shared_ptr<Person> mInterviewee;
    std::shared_ptr<Matter> mMatter;
    std::shared_ptr<PatentMatter> pMatter;

    // Widgets
    QLabel *lAppointmentType;
    PushButton *bDone;
    DateEdit *cDate;
    DateEdit *ccDate;
    LabeledWidgetLeft *lDate;
    TimeEdit *cTime;
    TimeEdit *ccTime;
    LabeledWidgetLeft *lTime;
    ExaminerInfoPanel *pExaminerInfo;
    AppointmentTaskPanel *pTask;
    FlagsPanel *pFlags;
    CollapseButton *collapseButton;       // Added for collapse feature
    DocketNumberButton *docketNumberButton;  // Added for collapsed view
    DynamicStackedWidget *stackedWidget;  // Added for state management

protected slots:
    void markDone();
    void emailExaminer();  // Fixed typo from emailExmainer
    void changeDate(const QDate& newdate);
    void changeTime(const QTime& newtime);

private slots:
    void onCollapseButtonToggled();  // Added to handle collapse button toggles

private:
    void Initialize();
    void connectSignalsAndSlots();   // Renamed for consistency
};

} // namespace Nutmeg

#endif // APPOINTMENTDASHPANEL_H
