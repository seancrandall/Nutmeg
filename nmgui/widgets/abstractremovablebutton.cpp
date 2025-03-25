#include "abstractremovablebutton.h"

namespace Nutmeg
{

AbstractRemovableButton::AbstractRemovableButton(QWidget *parent) : QWidget{parent}
{
    setupDisplay();
    connectSignalsAndSlots();
}

void AbstractRemovableButton::slotRemove()
{
    // InformationBox infoBox;
    // infoBox.text = "Remove object " + QString::number(mKey);
    // infoBox.exec();
    emit signalRemoveObject(mKey);
}

void AbstractRemovableButton::slotOpen()
{
    // InformationBox infoBox;
    // infoBox.text = "Open Object " + QString::number(mKey);
    // infoBox.exec();
    emit signalOpenObject(mKey);
}

void AbstractRemovableButton::setupDisplay()
{
    cObject->setText("Placeholder");

    hlayout->setSpacing(0);
    hlayout->addWidget(cObject);
    hlayout->addWidget(cRemove);

    setLayout(hlayout);
}

void AbstractRemovableButton::connectSignalsAndSlots()
{
    QObject::connect(cObject, &QPushButton::clicked, this, &AbstractRemovableButton::slotOpen);

    QObject::connect(cRemove, &QPushButton::clicked, this, &AbstractRemovableButton::slotRemove);
}

} // namespace Nutmeg
