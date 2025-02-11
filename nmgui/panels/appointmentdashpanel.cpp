#include "appointmentdashpanel.h"
#include "objects/patentmatter.h"
#include "windows/entitydialog.h"

namespace Nutmeg {

AppointmentDashPanel::AppointmentDashPanel(QWidget *parent)
    : Frame(parent)
{
    //Initialize();
}

AppointmentDashPanel::AppointmentDashPanel(Key appointmentId, QWidget *parent)
    : Frame(parent)
{
    mAppointment = std::make_shared<Appointment>(appointmentId);
    mTask = std::make_shared<Task>(mAppointment->associatedObject);
    PatentMatter mat(mTask->fkMatter);
    mInterviewee = std::make_shared<Person>(mat.fkExaminer);

    Initialize();
}

AppointmentDashPanel::AppointmentDashPanel(std::shared_ptr<Appointment> pAppt, QWidget *parent)
    : Frame(parent)
{
    mAppointment = pAppt;
    mTask = std::make_shared<Task>(mAppointment->associatedObject);
    PatentMatter mat(mTask->fkMatter);
    mInterviewee = std::make_shared<Person>(mat.fkExaminer);

    Initialize();
}

void AppointmentDashPanel::emailExaminer()
{

}

void AppointmentDashPanel::openExaminer()
{
    EntityDialog diag(mInterviewee->EntityId, this);
    diag.setModal(true);
    diag.exec();
}

void AppointmentDashPanel::markDone()
{
    mAppointment->complete = true;
}

void AppointmentDashPanel::InitializeControls()
{
    lAppointmentType = new QLabel();
    lAppointmentType->setText(mAppointment->typeString);

    bDone = new PushButton();
    bDone->setText("Done");

    //Examiner Information
    bExaminerName = new PushButton();
    bExaminerName->setText(mInterviewee->EntityName);

    cExaminerPhoneNumber = new LineEdit();
    cExaminerPhoneNumber->setText(mInterviewee->PrimaryPhone);
    lExaminerPhoneNumber = new LabeledWidgetLeft("Phone", cExaminerPhoneNumber);

    cExaminerEmail = new LineEdit();
    cExaminerEmail->setText(mInterviewee->PrimaryEmail);
    lExaminerEmail = new LabeledWidgetLeft("Email", cExaminerEmail);
    bEmailExaminer = new PushButton();
    bEmailExaminer->setText("✉");
    lExaminerEmail->addWidget(bEmailExaminer);


    pTask = new AppointmentTaskPanel(mAppointment->AppointmentId);
    pFlags = new FlagsPanel(mAppointment->AppointmentId);
}

void AppointmentDashPanel::LayoutWidgets()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->addWidget(lAppointmentType);
    mainLayout->addWidget(bDone);

    QVBoxLayout *examinerLayout = new QVBoxLayout();
    examinerLayout->addWidget(bExaminerName);
    examinerLayout->addLayout(lExaminerPhoneNumber);
    examinerLayout->addLayout(lExaminerEmail);

    mainLayout->addLayout(examinerLayout);
    mainLayout->addWidget(pTask);
    mainLayout->addWidget(pFlags);
}

void AppointmentDashPanel::ConnectSignalsAndSlots()
{

}

void AppointmentDashPanel::Initialize()
{
    InitializeControls();
    LayoutWidgets();
    ConnectSignalsAndSlots();
}

} // namespace Nutmeg
