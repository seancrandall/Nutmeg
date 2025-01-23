#include "insertinventordialog.h"

namespace Nutmeg
{

InsertInventorDialog::InsertInventorDialog(QWidget *parent) : Nutmeg::InsertPersonDialog(parent)
{
    setWindowTitle("Insert a New Inventor");
}

void InsertInventorDialog::slotHandleAccepted()
{
    InsertPersonDialog::slotHandleAccepted();
    Nutdb::AddRoleToPerson(mNewkey, 11); // 11 is role inventor
}

} // namespace Nutmeg
