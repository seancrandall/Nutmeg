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
    // Default constructor does nothing significant
}

AppointmentDashPanel::AppointmentDashPanel(Key appointmentId, QWidget *parent)
    : Frame(parent)
{
    mAppointment = std::make_shared<Appointment>(appointmentId);
    mTask = std::make_shared<Task>(mAppointment->associatedObject);
    mMatter = std::make_shared<Matter>(mTask->fkMatter);
    pMatter = std::make_shared<PatentMatter>(mMatter->MatterId);
    mInterviewee = std::make_shared<Person>(pMatter->fkExaminer);
    Initialize();
}

AppointmentDashPanel::AppointmentDashPanel(std::shared_ptr<Appointment> pAppt, QWidget *parent)
    : Frame(parent)
{
    mAppointment = pAppt;
    mTask = std::make_shared<Task>(mAppointment->associatedObject);
    mMatter = std::make_shared<Matter>(mTask->fkMatter);
    pMatter = std::make_shared<PatentMatter>(mMatter->MatterId);
    mInterviewee = std::make_shared<Person>(pMatter->fkExaminer);
    Initialize();
}

void AppointmentDashPanel::Initialize()
{
    // Create all widgets
    collapseButton = new CollapseButton(CollapseButtonState::Collapsed, this);
    lAppointmentType = new QLabel(this);
    lAppointmentType->setText(mAppointment->typeString);
    lAppointmentType->setFont(QFont("Arial", 14, QFont::Bold));
    docketNumberButton = new DocketNumberButton(mMatter, this);
    cDate = new DateEdit(mAppointment->AppointmentTime.date(), this);
    cDate->setDisplayFormat("ddd MMMM d");
    cDate->setMaximumWidth(300);
    ccDate = new DateEdit(mAppointment->AppointmentTime.date(), this);
    ccDate->setDisplayFormat("ddd MMMM d");
    ccDate->setMaximumWidth(300);
    lDate = new LabeledWidgetLeft("Date", ccDate);
    cTime = new TimeEdit(mAppointment->AppointmentTime.time(), this);
    cTime->setMaximumWidth(300);
    ccTime = new TimeEdit(mAppointment->AppointmentTime.time(), this);
    ccTime->setMaximumWidth(300);
    lTime = new LabeledWidgetLeft("Time", ccTime);
    bDone = new PushButton(this);
    bDone->setText("Done");
    bDone->setMaximumWidth(60);
    pExaminerInfo = new ExaminerInfoPanel(mInterviewee->EntityId, this);
    pTask = new AppointmentTaskPanel(mAppointment->AppointmentId, this);
    pFlags = new FlagsPanel(mAppointment->AppointmentId, this);

    // Set background color
    QPalette pal;
    QDate apptDate(mAppointment->AppointmentTime.date());
    QColor background = Deadline::getDateColor(apptDate);
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);

    // Create DynamicStackedWidget for state management
    stackedWidget = new DynamicStackedWidget(this);

    // Collapsed page
    QWidget *collapsedPage = new QWidget();
    QHBoxLayout *collapsedLayout = new QHBoxLayout(collapsedPage);
    collapsedLayout->addWidget(docketNumberButton);
    collapsedLayout->addLayout(lDate);
    collapsedLayout->addLayout(lTime);
    collapsedLayout->addStretch();  // Aligns widgets to the left
    stackedWidget->addWidget(collapsedPage);

    // Expanded page
    QWidget *expandedPage = new QWidget();
    QHBoxLayout *expandedLayout = new QHBoxLayout(expandedPage);
    QVBoxLayout *dtLayout = new QVBoxLayout();  // Stack date, time, and done button
    dtLayout->addWidget(cDate, 0, Qt::AlignHCenter);
    dtLayout->addWidget(cTime, 0, Qt::AlignHCenter);
    dtLayout->addWidget(bDone, 0, Qt::AlignHCenter);
    expandedLayout->addLayout(dtLayout);
    expandedLayout->addWidget(pExaminerInfo);
    expandedLayout->addWidget(pTask);
    expandedLayout->addWidget(pFlags);
    stackedWidget->addWidget(expandedPage);

    // Set initial state to collapsed
    stackedWidget->setCurrentIndex(0);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(collapseButton);
    mainLayout->addWidget(lAppointmentType);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);

    connectSignalsAndSlots();
}

void AppointmentDashPanel::markDone()
{
    mAppointment->complete = true;
}

void AppointmentDashPanel::emailExaminer()  // Fixed typo from emailExmainer
{
    Settings set;
    const QString fromEmail = set.email;
    const QString toEmail = mInterviewee->PrimaryEmail;
    QDateTime apptTime = mAppointment->AppointmentTime;

    Email email;

    const QString apptTimeString = apptTime.toString("dddd MMMM d");
    const QString examinerLastName = mInterviewee->LastName;
    const QString serialNumber = pMatter->ApplicationSerialNumber;
    const QString docketNumber = mMatter->AttorneyDocketNumber;

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
    QTime oldTime = mAppointment->AppointmentTime.time();
    mAppointment->AppointmentTime = QDateTime(newdate, oldTime);
}

void AppointmentDashPanel::changeTime(const QTime &newtime)
{
    QDate oldDate = mAppointment->AppointmentTime.date();
    mAppointment->AppointmentTime = QDateTime(oldDate, newtime);
}

void AppointmentDashPanel::expand()
{
    collapseButton->expand();
}

void AppointmentDashPanel::collapse()
{
    collapseButton->collapse();
}

void AppointmentDashPanel::onCollapseButtonToggled()
{
    if (collapseButton->state == CollapseButtonState::Expanded) {
        stackedWidget->setCurrentIndex(1);  // Expanded page
    } else {
        stackedWidget->setCurrentIndex(0);  // Collapsed page
    }
}

void AppointmentDashPanel::connectSignalsAndSlots()
{
    QObject::connect(bDone, &PushButton::clicked,
                     this, &AppointmentDashPanel::markDone);
    QObject::connect(pExaminerInfo, &ExaminerInfoPanel::signalEmailExaminer,
                     this, &AppointmentDashPanel::emailExaminer);
    QObject::connect(cDate, &DateEdit::userDateChanged,
                     this, &AppointmentDashPanel::changeDate);
    QObject::connect(ccDate, &DateEdit::userDateChanged,
                     this, &AppointmentDashPanel::changeDate);
    QObject::connect(cTime, &TimeEdit::userTimeChanged,
                     this, &AppointmentDashPanel::changeTime);
    QObject::connect(ccTime, &TimeEdit::userTimeChanged,
                     this, &AppointmentDashPanel::changeTime);
    QObject::connect(collapseButton, &CollapseButton::toggled,
                     this, &AppointmentDashPanel::onCollapseButtonToggled);
}

} // namespace Nutmeg
