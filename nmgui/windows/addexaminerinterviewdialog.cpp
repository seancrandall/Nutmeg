#include "addexaminerinterviewdialog.h"
#include "objects/appointment.h"
#include "utils/icalbuilder.h"

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
    Appointment *appt = new Appointment(mAppointment, mTask->TaskId);
    appt->type = Nutmeg::ExaminerInterviewPatent;
    delete appt;

    IcalBuilder *builder = new IcalBuilder(mAppointment, mAppointment.addMSecs(1800));
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
