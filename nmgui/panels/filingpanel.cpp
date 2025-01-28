#include "filingpanel.h"

namespace Nutmeg
{

FilingPanel::FilingPanel(Key id, QWidget *parent)
    : QWidget(parent)
    , filing(new Filing(id, this))
    , matter(new Matter(filing->fkMatter, this))
{
    LoadData();
}

void FilingPanel::slotUpdateCompletion()
{
    emit signalSomethingChanged();
}

void FilingPanel::slotRefreshView()
{
    emit signalSomethingChanged();
}

void FilingPanel::ConnectSignalsAndSlots()
{
    QObject::connect(doneButton, &QPushButton::clicked,
                     doneButton, &Nutmeg::DoneButton::slotHandleClicked);
}

void FilingPanel::LoadData()
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
