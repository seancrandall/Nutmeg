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
    eiInfo.interviewTime = interviewTimeEditor->dateTime();
    interviewInformationPanel->setTextFormat(Qt::RichText);
    interviewInformationPanel->setText(eiInfo.html);
}

void AddExaminerInterviewDialog::slotGather()
{
    //Add a new EI for the matter with the given DateTime
    mAppointmentTime = interviewTimeEditor->dateTime();
    Appointment *appt = new Appointment(mAppointmentTime, mTask->TaskId);
    appt->type = Nutmeg::ExaminerInterviewPatent;
    delete appt;

    IcalBuilder *builder = new IcalBuilder(mAppointmentTime, mAppointmentTime.addMSecs(1800));
    QString EventName = QString("Examiner Interview for %1 (%2)")
                            .arg(mMatter->AttorneyDocketNumber)
                            .arg(mMatter->ApplicationSerialNumber);
    builder->eventName = EventName;

    //Build an Event Description
    Entity client = Entity(mMatter->fkClient);
    QString Description = QString("Examiner Interview");
    Description += QString("Date: %1").arg(mAppointmentTime.date().toString(Qt::ISODate));
    Description += QString("Time: %1").arg(mAppointmentTime.time().toString());
    Description += QString("Client: %1").arg(client.FullLegalName);
    Description += QString("Examiner: %1").arg(mExaminer->FullLegalName);
    Description += QString("Response Type: %1").arg(mTask->TaskTypeString);
    builder->description = Description;

    builder->openIcsFile();
}

void AddExaminerInterviewDialog::slotUpdateInterview(const QDateTime &newInterviewTime)
{
    Q_UNUSED(newInterviewTime); //scatter grabs it from the control anyway
    valid = true;
    slotScatter();
}

void AddExaminerInterviewDialog::setupDisplay()
{
    interviewTimeEditor = new QDateTimeEdit();
    interviewTimeEditor->setDateTime(QDateTime::currentDateTime());
    interviewTimeEditor->setDisplayFormat("ddd MMMM d yyyy hh:mm");
    interviewTimeEditor->setCalendarPopup(true);

    interviewInformationPanel = new QLabel();
    interviewInformationPanel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    verticalLayout = new QVBoxLayout(workspace);
    verticalLayout->addWidget(interviewTimeEditor);
    verticalLayout->addWidget(interviewInformationPanel);

    updateLayout();
}

void AddExaminerInterviewDialog::connectSignalsAndSlots()
{
    Dialog::connectSignalsAndSlots();

    QObject::connect(interviewTimeEditor,       &QDateTimeEdit::dateTimeChanged,
                     this,                      &AddExaminerInterviewDialog::slotUpdateInterview);
}


} // namespace Nutmeg
