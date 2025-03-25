#include "responsedashpanel.h"
#include "responsetaskpanel.h"
#include "windows/patentmatterdialog.h"

namespace Nutmeg
{

ResponseDashPanel::ResponseDashPanel(Key responseId, QWidget *parent)
    : Frame{parent}
{
    mResponse = std::make_shared<Response>(responseId);
    mMatter = std::make_shared<Matter>(mResponse->fkMatter);
    collapseButton = new CollapseButton(CollapseButtonState::Collapsed, this);
    loadData();
    connectSignalsAndSlots();
}

void ResponseDashPanel::loadData()
{
    mDeadline = std::make_shared<Deadline>(mResponse->fkDeadline);

    // Set background color
    QPalette pal;
    QColor background = mDeadline->color;
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);

    // Create widgets
    responseTypeLabel = new ResponseTypeLabel(mResponse, this);
    doneButton = new DoneButton(mResponse, this);
    taskPanel = new ResponseTaskPanel(mResponse, this);
    deadlinePanel = new DeadlinesPanel(mDeadline, this);
    entitiesPanel = new EntitiesPanel(mResponse->TaskId, this);
    flagsPanel = new FlagsPanel(mResponse->ResponseId, this);
    docketNumberButton = new DocketNumberButton(mMatter, this);
    taskTypeCombo = new TaskTypeCombo(mResponse, this);
    //nextDeadlineLabel = new QLabel("Next Deadline", this);
    dateEdit = new DateEdit(mDeadline->NextDeadline, this);
    labeledDateEdit = new LabeledWidgetLeft("Next Deadline", dateEdit);


    // Create QStackedWidget for state management
    stackedWidget = new DynamicStackedWidget(this);

    // Collapsed page
    QWidget *collapsedPage = new QWidget();
    QHBoxLayout *collapsedLayout = new QHBoxLayout(collapsedPage);
    collapsedLayout->addWidget(docketNumberButton);
    collapsedLayout->addWidget(taskTypeCombo);
    collapsedLayout->addLayout(labeledDateEdit);
    collapsedLayout->addStretch();
    stackedWidget->addWidget(collapsedPage);

    // Expanded page
    QWidget *expandedPage = new QWidget();
    QHBoxLayout *expandedLayout = new QHBoxLayout(expandedPage);
    expandedLayout->addWidget(doneButton);
    expandedLayout->addWidget(taskPanel);
    expandedLayout->addWidget(deadlinePanel);
    expandedLayout->addWidget(entitiesPanel);
    expandedLayout->addWidget(flagsPanel);
    stackedWidget->addWidget(expandedPage);

    // Initial state: collapsed
    stackedWidget->setCurrentIndex(0);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(collapseButton);
    mainLayout->addWidget(responseTypeLabel);
    mainLayout->addWidget(stackedWidget);

    connectSignalsAndSlots();
}

void ResponseDashPanel::connectSignalsAndSlots()
{
    QObject::connect(doneButton, &QPushButton::clicked,
                     doneButton, &DoneButton::slotHandleClicked);
    QObject::connect(collapseButton, &CollapseButton::toggled,
                     this, &ResponseDashPanel::collapseButtonToggled);
    QObject::connect(taskTypeCombo,         &TaskTypeCombo::currentIndexChanged,
                     this,                   &ResponseDashPanel::changeResponseType);
    QObject::connect(dateEdit,              &DateEdit::dateChanged,
                     this,                   &ResponseDashPanel::changeNextDeadline);
}

void ResponseDashPanel::collapseButtonToggled()
{
    if (collapseButton->state == CollapseButtonState::Expanded) {
        stackedWidget->setCurrentIndex(1);  // Expanded page
        emit expanded();
    } else {
        stackedWidget->setCurrentIndex(0);  // Collapsed page
        emit collapsed();
    }
}

void ResponseDashPanel::expand()
{
    collapseButton->expand();
}

void ResponseDashPanel::collapse()
{
    collapseButton->collapse();
}

void ResponseDashPanel::changeResponseType()
{
    mResponse->fkTaskType = taskTypeCombo->key;
}

void ResponseDashPanel::changeNextDeadline()
{
    mDeadline->NextDeadline = dateEdit->getDate();
}

void ResponseDashPanel::updateParalegal(Key newkey)
{
    mResponse->fkParalegal = newkey;
}

void ResponseDashPanel::updateWorkAttorney(Key newkey)
{
    mResponse->fkWorkAttorney = newkey;
}

void ResponseDashPanel::updateClient(Key newkey)
{
    mMatter->fkClient = newkey;
}

} // namespace Nutmeg
