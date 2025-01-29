#include "deadlinespanel.h"

namespace Nutmeg
{

DeadlinesPanel::DeadlinesPanel(std::shared_ptr<Deadline> deadline, QWidget *parent)
    : Frame(parent)
    , mDeadline(deadline)
{
    Initialize();
}

DeadlinesPanel::DeadlinesPanel(std::shared_ptr<Deadline> deadline, bool updateInstantly, QWidget *parent)
    : Frame(parent)
    , instantUpdate(updateInstantly)
    , mDeadline(deadline)
{
    Initialize();
}

void DeadlinesPanel::Scatter()
{
    triggerEdit->date = mDeadline->TriggerDate;
    softEdit->date = mDeadline->SoftDeadline;
    hardEdit->date = mDeadline->HardDeadline;
    nextEdit->date = mDeadline->NextDeadline;
}

void DeadlinesPanel::Gather()
{
    mDeadline->holdTriggerDate(triggerEdit->date);
    mDeadline->holdSoftDeadline(softEdit->date);
    mDeadline->holdHardDeadline(hardEdit->date);
    mDeadline->holdNextDeadline(nextEdit->date);
    mDeadline->Commit();
}

void DeadlinesPanel::slotUpdateTriggerDate() {
    if(instantUpdate) mDeadline->TriggerDate = triggerEdit->date;
}

void DeadlinesPanel::slotUpdateNextDeadline()
{
    if(instantUpdate){
    QDate tempDate = nextEdit->date;
    mDeadline->NextDeadline = tempDate;
    }
}

void DeadlinesPanel::slotUpdateSoftDeadline()
{
    if(instantUpdate) mDeadline->SoftDeadline = softEdit->date;
}

void DeadlinesPanel::slotUpdateHardDeadline()
{
    if(instantUpdate) mDeadline->HardDeadline = hardEdit->date;
}

void DeadlinesPanel::ConnectSignalsAndSlots()
{
    QObject::connect(triggerEdit, &QDateEdit::editingFinished, this, &Nutmeg::DeadlinesPanel::slotUpdateTriggerDate);

    QObject::connect(nextEdit, &QDateEdit::editingFinished, this, &Nutmeg::DeadlinesPanel::slotUpdateNextDeadline);

    QObject::connect(softEdit, &QDateEdit::editingFinished, this, &Nutmeg::DeadlinesPanel::slotUpdateSoftDeadline);
    QObject::connect(hardEdit, &QDateEdit::editingFinished, this, &Nutmeg::DeadlinesPanel::slotUpdateHardDeadline);
}

void DeadlinesPanel::Initialize()
{
    //QVBoxLayout *layout = new QVBoxLayout(this);
    QGridLayout *layout = new QGridLayout(this);

    QVBoxLayout *triggerLayout = new QVBoxLayout();
    triggerLayout->setSpacing(1);
    QVBoxLayout *nextLayout = new QVBoxLayout();
    nextLayout->setSpacing(1);
    QVBoxLayout *softLayout = new QVBoxLayout();
    softLayout->setSpacing(1);
    QVBoxLayout *hardLayout = new QVBoxLayout();
    hardLayout->setSpacing(1);

    triggerEdit = new Nutmeg::DateEdit(mDeadline->TriggerDate);
    nextEdit = new Nutmeg::DateEdit(mDeadline->NextDeadline);
    softEdit = new Nutmeg::DateEdit(mDeadline->SoftDeadline);
    hardEdit = new Nutmeg::DateEdit(mDeadline->HardDeadline);

    QLabel *triggerLabel = new QLabel("Trigger Date");
    QLabel *nextLabel = new QLabel("Next Deadline");
    QLabel *softLabel = new QLabel("Soft Deadline");
    QLabel *hardLabel = new QLabel("Hard Deadline");

    triggerLayout->addWidget(triggerLabel);
    triggerLayout->addWidget(triggerEdit);
    triggerLayout->addStretch();

    nextLayout->addWidget(nextLabel);
    nextLayout->addWidget(nextEdit);
    nextLayout->addStretch();

    softLayout->addWidget(softLabel);
    softLayout->addWidget(softEdit);
    softLayout->addStretch();

    hardLayout->addWidget(hardLabel);
    hardLayout->addWidget(hardEdit);
    hardLayout->addStretch();

    layout->addLayout(triggerLayout, 0, 0);
    layout->addLayout(nextLayout, 0, 1);
    layout->addLayout(softLayout, 1, 0);
    layout->addLayout(hardLayout, 1, 1);

    ConnectSignalsAndSlots();
}

} // namespace Nutmeg
