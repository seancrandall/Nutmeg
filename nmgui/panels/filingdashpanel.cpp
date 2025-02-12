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

void FilingDashPanel::ConnectSignalsAndSlots()
{
    QObject::connect(doneButton, &QPushButton::clicked,
                     doneButton, &Nutmeg::DoneButton::slotHandleClicked);

    QObject::connect(taskPanel,     &Nutmeg::FilingTaskPanel::signalEmailInventors,
                    this,           &Nutmeg::FilingDashPanel::EmailInventors);
}

void FilingDashPanel::Initialize()
{
    deadline = std::make_shared<Deadline>(filing->fkDeadline);
    SetColor();
    InitializeControls();
    LayoutWidgets();
    ConnectSignalsAndSlots();
}

void FilingDashPanel::SetColor()
{
    QPalette pal;
    QColor background = Deadline(filing->fkDeadline).color;
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);
}

void FilingDashPanel::InitializeControls()
{
    label = new FilingTypeLabel(filing);
    doneButton = new DoneButton(filing);
    taskPanel = new FilingTaskPanel(filing);
    deadlinePanel = new DeadlinesPanel(deadline);
    entitiesPanel = new EntitiesPanel(filing->FilingId);
    flagsPanel = new FlagsPanel(filing->FilingId);
}

void FilingDashPanel::LayoutWidgets()
{
    QHBoxLayout *layout;
    layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(doneButton);
    layout->addWidget(taskPanel);
    layout->addWidget(deadlinePanel);
    layout->addWidget(entitiesPanel);
    layout->addWidget(flagsPanel);
}

} // namespace Nutmeg
