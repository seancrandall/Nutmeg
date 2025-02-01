#include "addexaminerinterviewdialog.h"
#include "objects/appointment.h"
#include "utils/icalbuilder.h"
#include "objects/entity.h"

namespace Nutmeg
{

AddExaminerInterviewDialog::AddExaminerInterviewDialog(Key responseId, QWidget *parent)
    : Dialog(parent)
    , mTask(std::make_shared<Task>(responseId))
    , mAttorney(std::make_shared<Person>(mTask->fkWorkAttorney))
    , mMatter(std::make_shared<PatentMatter>(mTask->fkMatter))
    , mExaminer(std::make_shared<Person>(mMatter->fkExaminer))
    , eiInfo(mTask->TaskId, mAttorney->PersonId)
    , mAppointmentDate(QDate::currentDate())
    , mAppointmentTime(QTime::currentTime())
    , mAppointmentDateTime(QDateTime::currentDateTime())
{
    setupDisplay();
    connectSignalsAndSlots();

    Settings settings(this);
    QUrl url(settings.airUrl);
    QDesktopServices::openUrl(url);
    slotScatter();
    show();
}

void AddExaminerInterviewDialog::slotHandleRejected()
{
    close();
}

void AddExaminerInterviewDialog::slotHandleAccepted()
{
    if(!valid){
        WarningBox box = WarningBox("Oops! Looks like you didn't specify a date and time!", this);
        box.exec();
        return;
    }

    slotGather();
    close();
}

void AddExaminerInterviewDialog::slotScatter()
{
    dateEditor->date = mAppointmentDate;
    timeEditor->time = mAppointmentTime;
    mAppointmentDateTime.setDate(mAppointmentDate);
    mAppointmentDateTime.setTime(mAppointmentTime);

    eiInfo.interviewTime = mAppointmentDateTime;
    interviewInformationPanel->setTextFormat(Qt::RichText);
    interviewInformationPanel->setText(eiInfo.html);
}

void AddExaminerInterviewDialog::slotGather()
{
    Settings set;
    //Add a new EI for the matter with the given DateTime
    //mAppointmentTime = interviewTimeEditor->dateTime();
    mAppointmentDate = dateEditor->date;
    mAppointmentTime = timeEditor->time;
    mAppointmentDateTime.setDate(mAppointmentDate);
    mAppointmentDateTime.setTime(mAppointmentTime);

    Appointment *appt = new Appointment(mAppointmentDateTime, mTask->TaskId);
    appt->type = Nutmeg::ExaminerInterviewPatent;
    delete appt;

    IcalBuilder *builder = new IcalBuilder(mAppointmentDateTime, mAppointmentDateTime.addMSecs(1800));

    builder->companyName = set.companyName;
    builder->productName = set.softwareName;

    QString EventName = QString("Examiner Interview for %1 (%2)")
                            .arg(mMatter->AttorneyDocketNumber)
                            .arg(mMatter->ApplicationSerialNumber);
    builder->eventName = EventName;

    //Build an Event Description
    Entity client = Entity(mMatter->fkClient);
    QString Description = QString("Examiner Interview");
    Description += QString("Date: %1").arg(mAppointmentDateTime.date().toString(Qt::ISODate));
    Description += QString("Time: %1").arg(mAppointmentDateTime.time().toString());
    Description += QString("Client: %1").arg(client.FullLegalName);
    Description += QString("Examiner: %1").arg(mExaminer->FullLegalName);
    Description += QString("Response Type: %1").arg(mTask->TaskTypeString);
    builder->description = Description;

    builder->openIcsFile();
    mTask->NeedsExaminerInterview = false;
}


void AddExaminerInterviewDialog::slotTimeChanged()
{
    valid = true;
    mAppointmentTime = timeEditor->getTime();
    mAppointmentDateTime = QDateTime(mAppointmentDate, mAppointmentTime);
    eiInfo.interviewTime = mAppointmentDateTime;
    interviewInformationPanel->setText(eiInfo.html);
}

void AddExaminerInterviewDialog::slotDateChanged()
{
    valid = true;
    mAppointmentDate = dateEditor->getDate();
    mAppointmentDateTime = QDateTime(mAppointmentDate, mAppointmentTime);
    eiInfo.interviewTime = mAppointmentDateTime;
    interviewInformationPanel->setText(eiInfo.html);
}


void AddExaminerInterviewDialog::setupDisplay()
{
    dateEditor = new DateEdit();
    timeEditor = new TimeEdit();
    dateEditor->date = QDate::currentDate();
    timeEditor->setTime(QTime::currentTime());

    interviewInformationPanel = new QLabel();
    interviewInformationPanel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    dtLayout = new QHBoxLayout();
    dtLayout->addWidget(dateEditor);
    dtLayout->addWidget(timeEditor);

    verticalLayout = new QVBoxLayout(workspace);
    verticalLayout->addLayout(dtLayout);
    verticalLayout->addWidget(interviewInformationPanel);

    updateLayout();
}

void AddExaminerInterviewDialog::connectSignalsAndSlots()
{
    Dialog::connectSignalsAndSlots();

    QObject::connect(dateEditor,                &Nutmeg::DateEdit::dateChanged,
                     this,                      &AddExaminerInterviewDialog::slotDateChanged);

    QObject::connect(timeEditor,                &Nutmeg::TimeEdit::timeChanged,
                     this,                      &AddExaminerInterviewDialog::slotTimeChanged);


}


} // namespace Nutmeg
