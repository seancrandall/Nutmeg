#include "patentmatterdialog.h"
#include "qscrollarea.h"
#include "widgets/inventorbutton.h"
#include "windows/insertentitydialog.h"
#include "windows/insertinventordialog.h"
#include "windows/insertpatentexaminerdialog.h"

namespace Nutmeg
{

PatentMatterDialog::PatentMatterDialog(Key pmkey, QWidget *parent)
    : Dialog(parent)
    , pmatter(new PatentMatter(pmkey, this))
{
    setupDisplay();
    slotScatter();
    connectSignalsAndSlots();
}

PatentMatterDialog::PatentMatterDialog(PatentMatter* matterin, QWidget *parent)
    : Dialog(parent)
    , pmatter(matterin)
{
    setupDisplay();
    slotScatter();
    connectSignalsAndSlots();
}

void PatentMatterDialog::slotScatter()
{
    mPanel->slotScatter();
    pmPanel->slotScatter();
}

void PatentMatterDialog::slotGather()
{
    mPanel->slotGather();
    pmPanel->slotGather();
}

void PatentMatterDialog::slotHandleRejected()
{
    Dialog::slotHandleRejected();
}

void PatentMatterDialog::slotHandleAccepted()
{
    Dialog::slotHandleAccepted();
}

void PatentMatterDialog::connectSignalsAndSlots()
{
    Dialog::connectSignalsAndSlots();
}

void PatentMatterDialog::setupDisplay()
{
    setWindowTitle("Patent Matter " + QString::number(pmatter->PatentMatterId));
    mPanel = new MatterPanel(pmatter, this);
    pmPanel = new PatentMatterPanel(pmatter, this);

    workLayout->addWidget(mPanel, 0, 0);
    workLayout->addWidget(pmPanel, 1, 0);
    updateLayout();
}


} // namespace Nutmeg
