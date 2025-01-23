#include "addexaminerinterviewdialog.h"

namespace Nutmeg
{

AddExaminerInterviewDialog::AddExaminerInterviewDialog(Key responseId, QWidget *parent)
    : Dialog(parent)
    , mResponse(responseId, this)
    , mAttorney(mResponse.fkWorkAttorney)
    , mMatter(mResponse.fkMatter, this)
    , mExaminer(mMatter.fkExaminer, this)
    , eiInfo(mResponse.ResponseId, mAttorney.PersonId)
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
}

void AddExaminerInterviewDialog::slotUpdateInterview(const QDateTime &newInterviewTime)
{
    Q_UNUSED(newInterviewTime); //scatter grabs it from the control anyway
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
