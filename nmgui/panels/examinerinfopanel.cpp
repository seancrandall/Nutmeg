#include "examinerinfopanel.h"
#include "settings.h"
#include "windows/entitydialog.h"

namespace Nutmeg {

ExaminerInfoPanel::ExaminerInfoPanel(Key examinerId, QWidget *parent)
    : Frame(parent)
    , mExaminer(std::make_shared<Person>(examinerId))
{
    Initialize();
}

ExaminerInfoPanel::ExaminerInfoPanel(std::shared_ptr<Person> pExaminer, QWidget *parent)
    : Frame(parent)
    , mExaminer(pExaminer)
{
    Initialize();
}

void ExaminerInfoPanel::emailExaminer()
{
    emit signalEmailExaminer();
}

void ExaminerInfoPanel::changeEmail()
{
    mExaminer->PrimaryEmail = cExaminerEmail->text;
}

void ExaminerInfoPanel::changePhoneNumber()
{
    mExaminer->PrimaryPhone = cExaminerPhoneNumber->text;
}

void ExaminerInfoPanel::openExaminerInfo()
{
    EntityDialog diag(mExaminer->EntityId);
    diag.setModal(true);
    diag.exec();
}

void ExaminerInfoPanel::Initialize()
{
    InitializeControls();
    LayoutControls();
    ConnectSignalsAndSlots();
}

void ExaminerInfoPanel::ConnectSignalsAndSlots()
{
    QObject::connect(bExaminerName,     &PushButton::clicked,
                     this,               &ExaminerInfoPanel::openExaminerInfo);

    QObject::connect(cExaminerPhoneNumber,  &LineEdit::editingFinished,
                     this,                   &ExaminerInfoPanel::changePhoneNumber);

    QObject::connect(cExaminerEmail,        &LineEdit::editingFinished,
                     this,                   &ExaminerInfoPanel::changeEmail);

    QObject::connect(bEmailExaminer,        &PushButton::clicked,
                     this,                   &ExaminerInfoPanel::emailExaminer);
}

void ExaminerInfoPanel::InitializeControls()
{
    //Examiner Information
    bExaminerName = new PushButton();
    bExaminerName->setText(mExaminer->EntityName);
    //bExaminerName->setMaximumWidth(200);

    cExaminerPhoneNumber = new LineEdit();
    cExaminerPhoneNumber->setText(mExaminer->PrimaryPhone);
    lExaminerPhoneNumber = new LabeledWidgetLeft("Phone", cExaminerPhoneNumber);

    cExaminerEmail = new LineEdit();
    cExaminerEmail->setText(mExaminer->PrimaryEmail);
    lExaminerEmail = new LabeledWidgetLeft("Email", cExaminerEmail);
    bEmailExaminer = new PushButton();
    bEmailExaminer->setText("✉");
    bEmailExaminer->setMaximumWidth(35);
    lExaminerEmail->addWidget(bEmailExaminer);
}



void ExaminerInfoPanel::LayoutControls()
{
    QVBoxLayout *examinerLayout = new QVBoxLayout(this);
    examinerLayout->addWidget(bExaminerName, 0, Qt::AlignHCenter);
    examinerLayout->addLayout(lExaminerPhoneNumber, Qt::AlignHCenter);
    examinerLayout->addLayout(lExaminerEmail, Qt::AlignHCenter);

    setMaximumWidth(350);
}

} // namespace Nutmeg
