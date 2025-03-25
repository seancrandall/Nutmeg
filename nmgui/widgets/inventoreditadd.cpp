#include "inventoreditadd.h"
#include "widgets/inventorssearchbox.h"

namespace Nutmeg
{

InventorEditAdd::InventorEditAdd(QWidget *parent) : ComboEditAdd(parent)
{
    delete cCombo;
    cCombo = new Nutmeg::InventorsSearchBox();
    setupDisplay();
    connectSignalsAndSlots();
}

InventorEditAdd::InventorEditAdd(QString label, QWidget *parent) : ComboEditAdd(label, parent)
{
    delete cCombo;
    cCombo = new Nutmeg::InventorsSearchBox();
    setupDisplay();
    connectSignalsAndSlots();
}

} // namespace Nutmeg
