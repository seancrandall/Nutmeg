#include "matterdialog.h"

namespace Nutmeg
{

MatterDialog::MatterDialog(Key matterid, QWidget *parent)
    : Nutmeg::Dialog(parent)
    , matter(Matter(matterid))
    , panel(new MatterPanel(&matter, this))
{
    MatterDialog::setupDisplay();
    MatterDialog::connectSignalsAndSlots();
    MatterDialog::slotScatter();
    layoutWorkspace();
    connectSignalsAndSlots();
}


void MatterDialog::setupDisplay(void)
{
    setWindowTitle("Matter " + matter.AttorneyDocketNumber);
}

void MatterDialog::connectSignalsAndSlots(void)
{
    Dialog::connectSignalsAndSlots();
}

void MatterDialog::slotScatter(void)
{
    panel->slotScatter();
}

void MatterDialog::slotGather(void)
{
    panel->slotGather();
}

void MatterDialog::layoutWorkspace()
{
    workLayout->addWidget(panel, 0, 0);
}

} // namespace Nutmeg
