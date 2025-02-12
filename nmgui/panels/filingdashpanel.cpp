#include "filingdashpanel.h"

namespace Nutmeg
{

FilingDashPanel::FilingDashPanel(Key id, QWidget *parent)
    : Frame(parent)
    , filing(std::make_shared<Filing>(id))
    , matter(std::make_shared<Matter>(filing->fkMatter))
{
    LoadData();
}

void FilingDashPanel::slotUpdateCompletion()
{
    emit signalSomethingChanged();
}

void FilingDashPanel::slotRefreshView()
{
    emit signalSomethingChanged();
}

void FilingDashPanel::ConnectSignalsAndSlots()
{
    QObject::connect(doneButton, &QPushButton::clicked,
                     doneButton, &Nutmeg::DoneButton::slotHandleClicked);
}

void FilingDashPanel::LoadData()
{
    deadline = std::make_shared<Deadline>(filing->fkDeadline);

    // Set color according to how close the deadline is
    QPalette pal;
    QColor background = Deadline(filing->fkDeadline).color;
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);

    QLabel *label = new FilingTypeLabel(filing);
    doneButton = new DoneButton(filing);
    FilingTaskPanel *taskPanel = new FilingTaskPanel(filing);
    DeadlinesPanel *deadlinePanel = new DeadlinesPanel(deadline);
    EntitiesPanel *entitiesPanel = new EntitiesPanel(filing->FilingId);
    FlagsPanel *flagsPanel = new FlagsPanel(filing->FilingId);

    QHBoxLayout *layout;
    layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(doneButton);
    layout->addWidget(taskPanel);
    layout->addWidget(deadlinePanel);
    layout->addWidget(entitiesPanel);
    layout->addWidget(flagsPanel);

    ConnectSignalsAndSlots();
}

} // namespace Nutmeg
