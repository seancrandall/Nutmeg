#include "responsepanel.h"
#include "responsetaskpanel.h"

namespace Nutmeg
{

ResponsePanel::ResponsePanel(Key responseId, QWidget *parent)
    : QWidget{parent}
    , response(std::make_shared<Response>(responseId))
    , matter(std::make_shared<Matter>(response->fkMatter))
{
    LoadData();
}

void ResponsePanel::LoadData(void)
{
    // Objects
    deadline = std::make_shared<Deadline>(response->fkDeadline);

    // Set Color
    QPalette pal;
    QColor background = Deadline(response->fkDeadline).color;
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);

    // Change window color according to deadline
    QLabel *label = new ResponseTypeLabel(response);
    doneButton = new DoneButton(response);
    ResponseTaskPanel *taskPanel = new ResponseTaskPanel(response);
    DeadlinesPanel *deadlinePanel = new DeadlinesPanel(deadline);
    EntitiesPanel *epanel = new EntitiesPanel(response->TaskId);
    FlagsPanel *fpanel = new FlagsPanel(response->ResponseId);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(doneButton);
    layout->addWidget(taskPanel);
    layout->addWidget(deadlinePanel);
    layout->addWidget(epanel);
    layout->addWidget(fpanel);

    ConnectSignalsAndSlots();
}

void ResponsePanel::slotUpdateParalegal(Key newkey)
{
    response->fkParalegal = newkey;
}

void ResponsePanel::slotUpdateWorkAttorney(Key newkey)
{
    response->fkWorkAttorney = newkey;
}

void ResponsePanel::slotUpdateClient(Key newkey)
{
    matter->fkClient = newkey;
}

void ResponsePanel::slotUpdateCompletion()
{
    emit signalSomethingChanged();
}

void ResponsePanel::slotRefreshView(void)
{
    emit signalSomethingChanged();
}

void ResponsePanel::ConnectSignalsAndSlots()
{
    QObject::connect(doneButton, &QPushButton::clicked,
                     doneButton, &Nutmeg::DoneButton::slotHandleClicked);
}

} // namespace Nutmeg
