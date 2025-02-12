#include <QDesktopServices>

#include "appointmentdashpanel.h"
#include "objects/patentmatter.h"
#include "settings.h"
#include "windows/entitydialog.h"
#include "utils/email.h"

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

void AppointmentDashPanel::markDone()
{
    mAppointment->complete = true;
}

void AppointmentDashPanel::emailExmainer()
{
    Settings set;
    const QString fromEmail = set.email;
    const QString toEmail = mInterviewee->PrimaryEmail;
    QDateTime apptTime = mAppointment->AppointmentTime;

    Email email;

    // Format the appointment time as "Day Month Date"
    const QString apptTimeString = apptTime.toString("dddd MMMM d");

    const QString examinerLastName = mInterviewee->LastName;
    PatentMatter mat(mTask->fkMatter);
    const QString serialNumber = mat.ApplicationSerialNumber;
    const QString docketNumber = mat.AttorneyDocketNumber;

    const QString subjectLine = QString("Examiner Interview for %1 (Our %2)")
                                    .arg(serialNumber)
                                    .arg(docketNumber);

    const QString emailBody = QString("Examiner %1,\n\nAttached is a claim markup for us to discuss during our interview on %2. Please let me know if you have any questions.\n\nThanks!\n")
                                  .arg(examinerLastName)
                                  .arg(apptTimeString);

    email.addRecipient(toEmail);
    email.from = fromEmail;
    email.subject = subjectLine;
    email.body = emailBody;

    email.open();
}

void AppointmentDashPanel::changeDate(const QDate &newdate)
{
    QTime oldTime = QTime(mAppointment->AppointmentTime.time());
    mAppointment->AppointmentTime = QDateTime(newdate, oldTime);
}

void AppointmentDashPanel::changeTime(const QTime &newtime)
{
    QDate oldDate = QDate(mAppointment->AppointmentTime.date());
    mAppointment->AppointmentTime = QDateTime(oldDate, newtime);
}

void AppointmentDashPanel::InitializeControls()
{
    lAppointmentType = new QLabel();
    lAppointmentType->setText(mAppointment->typeString);
    lAppointmentType->setFont(QFont("Arial", 14, QFont::Bold));

    bDone = new PushButton();
    bDone->setText("Done");
    bDone->setMaximumWidth(60);

    cDate = new DateEdit(mAppointment->AppointmentTime.date());
    cDate->setDisplayFormat("ddd MMMM d");
    cDate->setMaximumWidth(300);
    cTime = new TimeEdit(mAppointment->AppointmentTime.time());
    cTime->setMaximumWidth(300);

    pExaminerInfo = new ExaminerInfoPanel(mInterviewee->EntityId);

    pTask = new AppointmentTaskPanel(mAppointment->AppointmentId);
    pFlags = new FlagsPanel(mAppointment->AppointmentId);
}

void AppointmentDashPanel::LayoutWidgets()
{
    //Give it the right color
    QPalette pal;
    QDate apptDate(mAppointment->AppointmentTime.date());
    QColor background = Deadline::getDateColor(apptDate);
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);

    //Lay them out horizontally
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->addWidget(lAppointmentType);
    //mainLayout->addWidget(bDone);

    //Stack the date and time on each other
    QVBoxLayout *dtLayout = new QVBoxLayout();
    dtLayout->addWidget(cDate, 0, Qt::AlignHCenter);
    dtLayout->addWidget(cTime, 0, Qt::AlignHCenter);
    dtLayout->addWidget(bDone, 0, Qt::AlignHCenter);
    mainLayout->addLayout(dtLayout);

    mainLayout->addWidget(pExaminerInfo);

    mainLayout->addWidget(pTask);
    mainLayout->addWidget(pFlags);
}

void AppointmentDashPanel::ConnectSignalsAndSlots()
{
    QObject::connect(bDone,     &PushButton::clicked,
                    this,       &AppointmentDashPanel::markDone);

    QObject::connect(pExaminerInfo,     &ExaminerInfoPanel::signalEmailExaminer,
                     this,               &AppointmentDashPanel::emailExmainer);

    QObject::connect(cDate,         &DateEdit::userDateChanged,
                     this,           &AppointmentDashPanel::changeDate);

    QObject::connect(cTime,         &TimeEdit::userTimeChanged,
                     this,           &AppointmentDashPanel::changeTime);
}

void AppointmentDashPanel::Initialize()
{
    InitializeControls();
    LayoutWidgets();
    ConnectSignalsAndSlots();
}

} // namespace Nutmeg
