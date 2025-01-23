#include "insertpatentexaminerdialog.h"

namespace Nutmeg
{

InsertPatentExaminerDialog::InsertPatentExaminerDialog(QWidget *parent) : Nutmeg::InsertPersonDialog(parent)
{
    setWindowTitle("Insert a New Patent Examiner");
}

void InsertPatentExaminerDialog::slotHandleAccepted()
{
    InsertPersonDialog::slotHandleAccepted();
    Nutdb::AddRoleToPerson(mNewkey, 7); // 7 is patent examiner
}

} // namespace Nutmeg
