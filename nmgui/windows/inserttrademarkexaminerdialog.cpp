#include "inserttrademarkexaminerdialog.h"

namespace Nutmeg
{

InsertTrademarkExaminerDialog::InsertTrademarkExaminerDialog(QWidget *parent) : Nutmeg::InsertPersonDialog(parent)
{
    setWindowTitle("Insert a New Trademark Examiner");
}

void InsertTrademarkExaminerDialog::slotHandleAccepted()
{
    InsertPersonDialog::slotHandleAccepted();
    Nutdb::AddRoleToPerson(mNewkey, 16);
}

} // namespace Nutmeg
