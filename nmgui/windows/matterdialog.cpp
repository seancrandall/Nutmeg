#include "matterdialog.h"

namespace Nutmeg
{

MatterDialog::MatterDialog(Key matterid, QWidget *parent)
    : Nutmeg::Dialog(parent)
    , mMatter(std::make_shared<Matter>(matterid, this))
    , mPanel(new MatterPanel(mMatter, this))
{
    Initialize();
}

MatterDialog::MatterDialog(std::shared_ptr<Matter> matter, QWidget *parent)
    : Nutmeg::Dialog(parent)
    , mMatter(matter)
    , mPanel(new MatterPanel(mMatter, this))
{
    Initialize();
}


void MatterDialog::setupDisplay(void)
{
    setWindowTitle("Matter " + mMatter->AttorneyDocketNumber);
}

void MatterDialog::connectSignalsAndSlots(void)
{
    Dialog::connectSignalsAndSlots();
}

void MatterDialog::slotScatter(void)
{
    mPanel->slotScatter();
}

void MatterDialog::slotGather(void)
{
    mPanel->slotGather();
}

void MatterDialog::layoutWorkspace()
{
    workLayout->addWidget(mPanel, 0, 0);
}

void MatterDialog::Initialize()
{
    setupDisplay();
    connectSignalsAndSlots();
    slotScatter();
    layoutWorkspace();
    connectSignalsAndSlots();
}

} // namespace Nutmeg
