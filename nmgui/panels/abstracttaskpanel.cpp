#include "abstracttaskpanel.h"
#include "widgets/arrowbutton.h"
#include "windows/patentmatterdialog.h"

namespace Nutmeg
{

AbstractTaskPanel::AbstractTaskPanel(std::shared_ptr<Task> task, QWidget *parent)
    : Frame(parent)
      , mMatter(std::make_shared<Matter>(task->fkMatter))
      , mTask(task)
{
    SetupDisplay();
}

Nutmeg::AbstractTaskPanel::AbstractTaskPanel(PushButton *xtra, std::shared_ptr<Task>task, QWidget *parent)
    : Frame(parent)
      , mMatter(std::make_shared<Matter>(task->fkMatter))
      , mTask(task)
      , mExtraButton(xtra)
{
    SetupDisplay();
}

AbstractTaskPanel::AbstractTaskPanel(Key taskId, PushButton *xtra, QWidget *parent)
    : Frame(parent)
{
    mTask = std::make_shared<Task>(taskId);
    mMatter = std::make_shared<Matter>(mTask->fkMatter);
    extraButton = xtra;

    SetupDisplay();
}

AbstractTaskPanel::AbstractTaskPanel(Key appointmentId, QWidget *parent)
    : Frame(parent)
{
    Appointment app(appointmentId);
    Key objectId = app.associatedObject;

    mTask = std::make_shared<Task>(objectId);
    mMatter = std::make_shared<Matter>(mTask->fkMatter);

    //Display will be setup after child sets the "extra" button
    //If you don't want an extraButton, set it to nullptr
}

void AbstractTaskPanel::setExtraButton(PushButton *newButton)
{
    mExtraButton = newButton;
    SetupDisplay();
}

void Nutmeg::AbstractTaskPanel::ConnectSigalsAndSlots()
{
    QObject::connect(docketNumberButton, &QPushButton::clicked,
                    this,                &Nutmeg::AbstractTaskPanel::slotOpenMatterWindow);

    QObject::connect(titleEdit, &QTextEdit::textChanged,
                     this,   &Nutmeg::AbstractTaskPanel::slotUpdateTitle);

    QObject::connect(taskTypeCombo, &Nutmeg::TaskTypeCombo::signalKeyChanged,
                     this,          &Nutmeg::AbstractTaskPanel::slotUpdateTaskType);

    QObject::connect(mExtraButton, &QAbstractButton::clicked,
                     this, &Nutmeg::AbstractTaskPanel::slotExtraButtonAction);

    QObject::connect(openTaskButton,    &ArrowButton::clicked,
                     this,              &AbstractTaskPanel::slotOpenTaskWindow);


}

void Nutmeg::AbstractTaskPanel::slotUpdateTitle()
{
    mMatter->Title = titleEdit->toPlainText();
}

void Nutmeg::AbstractTaskPanel::slotUpdateTaskType(Key newval)
{
    mTask->fkTaskType = newval;
}

void AbstractTaskPanel::slotOpenTaskWindow()
{
    TaskDialog diag(mTask, this);
    diag.setModal(true);
    diag.exec();
}

void Nutmeg::AbstractTaskPanel::slotOpenMatterWindow()
{
    PatentMatterDialog mdiag(mMatter->MatterId, this);
    mdiag.exec();
    return;
}

void Nutmeg::AbstractTaskPanel::SetupDisplay()
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
    if (mExtraButton != nullptr)
        vLayout->addWidget(mExtraButton, 0, Qt::AlignHCenter);

    setMaximumWidth(600);

    ConnectSigalsAndSlots();
}

} // namespace Nutmeg
