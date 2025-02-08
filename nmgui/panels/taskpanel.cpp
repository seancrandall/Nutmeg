#include "taskpanel.h"
#include "widgets/arrowbutton.h"
#include "windows/patentmatterdialog.h"

namespace Nutmeg
{

TaskPanel::TaskPanel(std::shared_ptr<Task> task, QWidget *parent)
    : Frame(parent)
      , mMatter(std::make_shared<Matter>(task->fkMatter))
      , mTask(task)
{
    SetupDisplay();
}

Nutmeg::TaskPanel::TaskPanel(PushButton *xtra, std::shared_ptr<Task>task, QWidget *parent)
    : Frame(parent)
      , mMatter(std::make_shared<Matter>(task->fkMatter))
      , mTask(task)
      , extraButton(xtra)
{
    SetupDisplay();
}

void Nutmeg::TaskPanel::ConnectSigalsAndSlots()
{
    QObject::connect(docketNumberButton, &QPushButton::clicked,
                    this,                &Nutmeg::TaskPanel::slotOpenMatterWindow);

    QObject::connect(titleEdit, &QTextEdit::textChanged,
                     this,   &Nutmeg::TaskPanel::slotUpdateTitle);

    QObject::connect(taskTypeCombo, &Nutmeg::TaskTypeCombo::signalKeyChanged,
                     this,          &Nutmeg::TaskPanel::slotUpdateTaskType);

    QObject::connect(extraButton, &QAbstractButton::clicked,
                     this, &Nutmeg::TaskPanel::slotExtraButtonAction);

    QObject::connect(openTaskButton,    &ArrowButton::clicked,
                     this,              &TaskPanel::slotOpenTaskWindow);


}

void Nutmeg::TaskPanel::slotUpdateTitle()
{
    mMatter->Title = titleEdit->toPlainText();
}

void Nutmeg::TaskPanel::slotUpdateTaskType(Key newval)
{
    mTask->fkTaskType = newval;
}

void TaskPanel::slotOpenTaskWindow()
{
    TaskDialog diag(mTask, this);
    diag.setModal(true);
    diag.exec();
}

void Nutmeg::TaskPanel::slotOpenMatterWindow()
{
    PatentMatterDialog mdiag(mMatter->MatterId, this);
    mdiag.exec();
    return;
}

void Nutmeg::TaskPanel::SetupDisplay()
{
    docketNumberButton = new DocketNumberButton(mMatter, this);
    docketNumberButton->setFlat(true);

    QHBoxLayout *tasklayout = new QHBoxLayout();
    taskTypeCombo = new TaskTypeCombo(mTask, this);
    taskTypeCombo->setMaximumWidth(220);
    tasklayout->addWidget(taskTypeCombo);
    openTaskButton = new ArrowButton(this);
    tasklayout->addWidget(openTaskButton);
    tasklayout->setAlignment(Qt::AlignHCenter);

    titleEdit = new TitleEdit(mMatter, this);

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
