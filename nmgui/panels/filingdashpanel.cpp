#include "filingdashpanel.h"
#include "objects/patentmatter.h"

namespace Nutmeg
{

FilingDashPanel::FilingDashPanel(Key id, QWidget *parent)
    : Frame(parent)
    , filing(std::make_shared<Filing>(id))
    , matter(std::make_shared<Matter>(filing->fkMatter))
{
    Initialize();
}

void FilingDashPanel::slotUpdateCompletion()
{
    emit signalSomethingChanged();
}

void FilingDashPanel::slotRefreshView()
{
    emit signalSomethingChanged();
}

void FilingDashPanel::EmailInventors(const QString &body)
{
    PatentMatter m = PatentMatter(filing->fkMatter);
    m.EmailInventors(body);
}

void FilingDashPanel::Initialize()
{
    deadline = std::make_shared<Deadline>(filing->fkDeadline);
    SetColor();

    // Create all widgets
    collapseButton = new CollapseButton(CollapseButtonState::Collapsed, this);
    label = new FilingTypeLabel(filing, this);
    docketNumberButton = new DocketNumberButton(matter, this);
    taskCombo = new TaskTypeCombo(filing, this);
    dateEdit = new DateEdit(deadline->NextDeadline, this);
    labeledDateEdit = new LabeledWidgetLeft("Deadline", dateEdit);
    doneButton = new DoneButton(filing, this);
    taskPanel = new FilingTaskPanel(filing, this);
    deadlinePanel = new DeadlinesPanel(deadline, this);
    entitiesPanel = new EntitiesPanel(filing->FilingId, this);
    flagsPanel = new FlagsPanel(filing->FilingId, this);

    // Create DynamicStackedWidget for state management
    stackedWidget = new DynamicStackedWidget(this);

    // Collapsed page
    QWidget *collapsedPage = new QWidget();
    QHBoxLayout *collapsedLayout = new QHBoxLayout(collapsedPage);
    collapsedLayout->addWidget(docketNumberButton);
    collapsedLayout->addWidget(taskCombo);
    collapsedLayout->addLayout(labeledDateEdit);
    collapsedLayout->addStretch();  // Aligns widgets to the left
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

    // Set initial state to collapsed
    stackedWidget->setCurrentIndex(0);

    // Set up main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(collapseButton);
    mainLayout->addWidget(label);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);

    ConnectSignalsAndSlots();
}

void FilingDashPanel::SetColor()
{
    QPalette pal;
    QColor background = deadline->color;
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);
}

void FilingDashPanel::ConnectSignalsAndSlots()
{
    QObject::connect(doneButton, &QPushButton::clicked,
                     doneButton, &Nutmeg::DoneButton::slotHandleClicked);
    QObject::connect(taskPanel, &Nutmeg::FilingTaskPanel::signalEmailInventors,
                     this, &Nutmeg::FilingDashPanel::EmailInventors);
    QObject::connect(collapseButton, &CollapseButton::toggled,
                     this, &FilingDashPanel::onCollapseButtonToggled);
    QObject::connect(dateEdit, &DateEdit::dateChanged,
                     this, &FilingDashPanel::updateDeadline);
    QObject::connect(taskCombo, &QComboBox::currentIndexChanged,
                     this, &FilingDashPanel::updateFilingType);
}

void FilingDashPanel::onCollapseButtonToggled()
{
    if (collapseButton->state == CollapseButtonState::Expanded) {
        stackedWidget->setCurrentIndex(1);  // Expanded page
    } else {
        stackedWidget->setCurrentIndex(0);  // Collapsed page
    }
}

void FilingDashPanel::expand()
{
    collapseButton->expand();
}

void FilingDashPanel::collapse()
{
    collapseButton->collapse();
}

void FilingDashPanel::updateDeadline()
{
    deadline->NextDeadline = dateEdit->getDate();
}

void FilingDashPanel::updateFilingType()
{
    filing->fkTaskType = taskCombo->key;
}

} // namespace Nutmeg
