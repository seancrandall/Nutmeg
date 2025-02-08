#include "inventorbutton.h"

namespace Nutmeg
{

InventorButton::InventorButton(QWidget *parent) : AbstractRemovableButton{parent}, inventor(new Person())
{
    mKey = 0;
    setupInventorDisplay();
}

InventorButton::InventorButton(Key id, QWidget *parent) : AbstractRemovableButton(parent), inventor(new Person(id))
{
    mKey = id;
    setupInventorDisplay();
}

void InventorButton::setKey(Key newkey)
{
    mKey = newkey;
    if (inventor != nullptr)
        delete inventor;
    inventor = new Person(mKey);
}

void InventorButton::setupInventorDisplay()
{
    cObject->setText(inventor->EntityName);

    QObject::connect(this,  &AbstractRemovableButton::signalOpenObject,
                     this,   &InventorButton::slotReceiveOpenAction);

    QObject::connect(this,  &AbstractRemovableButton::signalRemoveObject,
                     this,   &InventorButton::slotReceiveRemoveAction);
}

void InventorButton::slotReceiveOpenAction()
{
    emit signalOpenInventor(inventor->PersonId);
}

void InventorButton::slotReceiveRemoveAction()
{
    emit signalRemoveInventor(inventor->PersonId);
}

} // namespace Nutmeg
