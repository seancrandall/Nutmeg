#include "entitydialog.h"

namespace Nutmeg {

EntityDialog::EntityDialog(Key entityId, QWidget *parent)
    : Dialog(parent)
{
    mEntity = std::make_shared<Entity>(entityId);
    Initialize();
}

EntityDialog::EntityDialog(std::shared_ptr<Entity> entity, QWidget *parent)
    : Dialog(parent)
{
    mEntity = entity;
    Initialize();
}

void EntityDialog::setupDisplay()
{
    setWindowTitle("Entity " + mEntity->EntityName);
}

void EntityDialog::connectSignalsAndSlots()
{
    Dialog::connectSignalsAndSlots();
}

void EntityDialog::slotGather()
{
    mPanel->gather();
}

void EntityDialog::slotScatter()
{
    mPanel->scatter();
}

void EntityDialog::LayoutWorkspace()
{
    workLayout->addWidget(mPanel, 0, 0);
    updateLayout();
}

void EntityDialog::Initialize()
{
    mPanel = new EntityPanel(mEntity);
    setupDisplay();
    connectSignalsAndSlots();
    slotScatter();
    LayoutWorkspace();
}



} // namespace Nutmeg
