#include "deadlinespanel.h"

namespace Nutmeg
{

DeadlinesPanel::DeadlinesPanel(Deadline *deadline, QWidget *parent)
    : Frame(parent)
    , mDeadline(deadline)
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

    triggerEdit = new Nutmeg::DateEdit(mDeadline->TriggerDate, this);
    nextEdit = new Nutmeg::DateEdit(mDeadline->NextDeadline, this);
    softEdit = new Nutmeg::DateEdit(mDeadline->SoftDeadline, this);
    hardEdit = new Nutmeg::DateEdit(mDeadline->HardDeadline, this);

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

void DeadlinesPanel::slotUpdateTriggerDate() {
    mDeadline->TriggerDate = triggerEdit->date;
}

void DeadlinesPanel::slotUpdateNextDeadline()
{
    QDate tempDate = nextEdit->date;
    mDeadline->NextDeadline = tempDate;
}

void DeadlinesPanel::slotUpdateSoftDeadline()
{
    mDeadline->SoftDeadline = softEdit->date;
}

void DeadlinesPanel::slotUpdateHardDeadline()
{
    mDeadline->HardDeadline = hardEdit->date;
}

void DeadlinesPanel::ConnectSignalsAndSlots()
{
    QObject::connect(triggerEdit, &QDateEdit::editingFinished, this, &Nutmeg::DeadlinesPanel::slotUpdateTriggerDate);

    QObject::connect(nextEdit, &QDateEdit::editingFinished, this, &Nutmeg::DeadlinesPanel::slotUpdateNextDeadline);

    QObject::connect(softEdit, &QDateEdit::editingFinished, this, &Nutmeg::DeadlinesPanel::slotUpdateSoftDeadline);
    QObject::connect(hardEdit, &QDateEdit::editingFinished, this, &Nutmeg::DeadlinesPanel::slotUpdateHardDeadline);
}

} // namespace Nutmeg
