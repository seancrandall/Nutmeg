#include "responsedashpanel.h"
#include "responsetaskpanel.h"

namespace Nutmeg
{

ResponseDashPanel::ResponseDashPanel(Key responseId, QWidget *parent)
    : Frame{parent}
{
    mResponse = std::make_shared<Response>(responseId);
    mMatter = std::make_shared<Matter>(mResponse->fkMatter);
    LoadData();
}

void ResponseDashPanel::LoadData(void)
{
    // Objects
    mDeadline = std::make_shared<Deadline>(mResponse->fkDeadline);

    // Set Color
    QPalette pal;
    QColor background = Deadline(mResponse->fkDeadline).color;
    pal.setColor(QPalette::Window, background);
    setPalette(pal);
    setAutoFillBackground(true);

    // Change window color according to deadline
    QLabel *label = new ResponseTypeLabel(mResponse);
    doneButton = new DoneButton(mResponse);
    ResponseTaskPanel *taskPanel = new ResponseTaskPanel(mResponse);
    DeadlinesPanel *deadlinePanel = new DeadlinesPanel(mDeadline);
    EntitiesPanel *epanel = new EntitiesPanel(mResponse->TaskId);
    FlagsPanel *fpanel = new FlagsPanel(mResponse->ResponseId);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(doneButton);
    layout->addWidget(taskPanel);
    layout->addWidget(deadlinePanel);
    layout->addWidget(epanel);
    layout->addWidget(fpanel);

    ConnectSignalsAndSlots();
}

void ResponseDashPanel::slotUpdateParalegal(Key newkey)
{
    mResponse->fkParalegal = newkey;
}

void ResponseDashPanel::slotUpdateWorkAttorney(Key newkey)
{
    mResponse->fkWorkAttorney = newkey;
}

void ResponseDashPanel::slotUpdateClient(Key newkey)
{
    mMatter->fkClient = newkey;
}

void ResponseDashPanel::slotUpdateCompletion()
{
    emit signalSomethingChanged();
}

void ResponseDashPanel::slotRefreshView(void)
{
    emit signalSomethingChanged();
}

void ResponseDashPanel::ConnectSignalsAndSlots()
{
    QObject::connect(doneButton, &QPushButton::clicked,
                     doneButton, &Nutmeg::DoneButton::slotHandleClicked);
}

} // namespace Nutmeg
