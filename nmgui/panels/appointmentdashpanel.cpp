#include <QDesktopServices>

#include "appointmentdashpanel.h"
#include "objects/patentmatter.h"
#include "settings.h"
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

    // Create the mailto link
    QString mailtoLink = QString("mailto:%1?subject=%2&body=%3")
                             .arg(toEmail)
                             .arg(QUrl::toPercentEncoding(subjectLine))
                             .arg(QUrl::toPercentEncoding(emailBody));

    // Open the link using QDesktopServices
    QDesktopServices::openUrl(QUrl(mailtoLink));
}

void AppointmentDashPanel::InitializeControls()
{
    lAppointmentType = new QLabel();
    lAppointmentType->setText(mAppointment->typeString);
    lAppointmentType->setFont(QFont("Arial", 14, QFont::Bold));

    bDone = new PushButton();
    bDone->setText("Done");
    bDone->setMaximumWidth(60);

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

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->addWidget(lAppointmentType);
    mainLayout->addWidget(bDone);

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
}

void AppointmentDashPanel::Initialize()
{
    InitializeControls();
    LayoutWidgets();
    ConnectSignalsAndSlots();
}

} // namespace Nutmeg
