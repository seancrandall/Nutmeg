#include "responsepanel.h"
#include "responsetaskpanel.h"

namespace Nutmeg
{

ResponsePanel::ResponsePanel(Key responseId, QWidget *parent)
    : QWidget{parent}
      , response(responseId)
      , task(response.ResponseId)
      , matter(response.fkMatter)
{
    LoadData();
}

void ResponsePanel::LoadData(void)
{
    // Objects
    deadline = std::make_unique<Deadline>(response.fkDeadline);

    // Set Color
    QPalette pal;
    QColor background = Deadline(task.fkDeadline).color;
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);

    // Change window color according to deadline
    QLabel *label = new ResponseTypeLabel(&response);
    doneButton = new DoneButton(&task);
    ResponseTaskPanel *taskPanel = new ResponseTaskPanel(&response);
    DeadlinesPanel *deadlinePanel = new DeadlinesPanel(deadline.get());
    EntitiesPanel *epanel = new EntitiesPanel(response.TaskId);
    FlagsPanel *fpanel = new FlagsPanel(response.ResponseId);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(doneButton);
    layout->addWidget(taskPanel);
    layout->addWidget(deadlinePanel);
    layout->addWidget(epanel);
    layout->addWidget(fpanel);

    ConnectSignalsAndSlots();
}

void ResponsePanel::slotUpdateParalegal(Key newkey) { task.fkParalegal = newkey; }

void ResponsePanel::slotUpdateWorkAttorney(Key newkey) { task.fkWorkAttorney = newkey; }

void ResponsePanel::slotUpdateClient(Key newkey) { matter.fkClient = newkey; }

void ResponsePanel::slotUpdateCompletion() { emit signalSomethingChanged(); }

void ResponsePanel::slotRefreshView(void) { emit signalSomethingChanged(); }

void ResponsePanel::ConnectSignalsAndSlots()
{
    QObject::connect(doneButton, &QPushButton::clicked,
                     doneButton, &Nutmeg::DoneButton::slotHandleClicked);
}

} // namespace Nutmeg
