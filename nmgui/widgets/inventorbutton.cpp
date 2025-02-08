#include "inventorbutton.h"

namespace Nutmeg
{

InventorButton::InventorButton(QWidget *parent) : AbstractRemovableButton{parent}

{
    mInventor = std::make_shared<Person>(0);
    mKey = 0;
    setupInventorDisplay();
}

InventorButton::InventorButton(Key id, QWidget *parent)
    : AbstractRemovableButton(parent)
{
    mInventor = std::make_shared<Person>(id);
    mKey = id;
    setupInventorDisplay();
}

void InventorButton::setKey(Key newkey)
{
    mKey = newkey;
    mInventor = std::make_shared<Person>(newkey);
}

void InventorButton::setupInventorDisplay()
{
    cObject->setText(mInventor->EntityName);

    QObject::connect(this,  &AbstractRemovableButton::signalOpenObject,
                     this,   &InventorButton::slotReceiveOpenAction);

    QObject::connect(this,  &AbstractRemovableButton::signalRemoveObject,
                     this,   &InventorButton::slotReceiveRemoveAction);
}

void InventorButton::slotReceiveOpenAction()
{
    emit signalOpenInventor(mInventor->PersonId);
}

void InventorButton::slotReceiveRemoveAction()
{
    emit signalRemoveInventor(mInventor->PersonId);
}

} // namespace Nutmeg
