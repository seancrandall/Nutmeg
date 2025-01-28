#include "responsepanel.h"
#include "responsetaskpanel.h"

namespace Nutmeg
{

ResponsePanel::ResponsePanel(Key responseId, QWidget *parent)
    : QWidget{parent}
    , mResponse(std::make_shared<Response>(responseId))
    , mMatter(std::make_shared<Matter>(mResponse->fkMatter))
{
    LoadData();
}

void ResponsePanel::LoadData(void)
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

void ResponsePanel::slotUpdateParalegal(Key newkey)
{
    mResponse->fkParalegal = newkey;
}

void ResponsePanel::slotUpdateWorkAttorney(Key newkey)
{
    mResponse->fkWorkAttorney = newkey;
}

void ResponsePanel::slotUpdateClient(Key newkey)
{
    mMatter->fkClient = newkey;
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
