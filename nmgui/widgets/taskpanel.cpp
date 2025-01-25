#include "taskpanel.h"
#include "widgets/arrowbutton.h"

namespace Nutmeg
{

TaskPanel::TaskPanel(Task *tsk, QWidget *parent)
    : Frame(parent)
      , matter(new Matter(tsk->fkMatter))
      , task(tsk)
{
    SetupDisplay();
}

Nutmeg::TaskPanel::TaskPanel(PushButton *xtra, Task *tsk, QWidget *parent)
    : Frame(parent)
      , matter(new Matter(tsk->fkMatter))
      , task(tsk)
      , extraButton(xtra)
{
    SetupDisplay();
}

void Nutmeg::TaskPanel::ConnectSigalsAndSlots()
{
    QObject::connect(docketNumberButton, &QPushButton::clicked, this, &Nutmeg::TaskPanel::slotOpenMatterWindow,
                     Qt::UniqueConnection);

    QObject::connect(titleEdit, &QTextEdit::textChanged, this, &Nutmeg::TaskPanel::slotUpdateTitle,
                     Qt::UniqueConnection);

    QObject::connect(taskTypeCombo, &Nutmeg::TaskTypeCombo::signalKeyChanged, this,
                     &Nutmeg::TaskPanel::slotUpdateTaskType, Qt::UniqueConnection);

    QObject::connect(extraButton, &QAbstractButton::clicked, this, &Nutmeg::TaskPanel::slotExtraButtonAction,
                     Qt::UniqueConnection);

    QObject::connect(openTaskButton,    &ArrowButton::clicked,
                     this,              &TaskPanel::slotOpenTaskWindow,
                    Qt::UniqueConnection);


}

void Nutmeg::TaskPanel::slotUpdateTitle() { matter->Title = titleEdit->toPlainText(); }

void Nutmeg::TaskPanel::slotUpdateTaskType(Key newval) { task->fkTaskType = newval; }

void TaskPanel::slotOpenTaskWindow()
{
    InformationBox info(this);
    info.text = "Open Task Window";
    info.exec();
}

void Nutmeg::TaskPanel::slotOpenMatterWindow()
{
    PatentMatterDialog mdiag(matter->MatterId, this);
    mdiag.exec();
    return;
}

void Nutmeg::TaskPanel::SetupDisplay()
{
    docketNumberButton = new DocketNumberButton(matter, this);
    docketNumberButton->setFlat(true);

    QHBoxLayout *tasklayout = new QHBoxLayout();
    taskTypeCombo = new TaskTypeCombo(task, this);
    taskTypeCombo->setMaximumWidth(220);
    tasklayout->addWidget(taskTypeCombo);
    openTaskButton = new ArrowButton(this);
    tasklayout->addWidget(openTaskButton);
    tasklayout->setAlignment(Qt::AlignHCenter);

    titleEdit = new TitleEdit(matter, this);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(10, 10, 10, 10); // Comfortable margin

    vLayout->addWidget(docketNumberButton, 0, Qt::AlignHCenter);
    vLayout->addLayout(tasklayout);
    vLayout->addWidget(titleEdit, 0, Qt::AlignHCenter);

    // Only add extra button if it's set
    if (extraButton != nullptr)
        vLayout->addWidget(extraButton, 0, Qt::AlignHCenter);

    setMaximumWidth(600);

    ConnectSigalsAndSlots();
}

} // namespace Nutmeg
