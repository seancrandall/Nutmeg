#include "messagebox.h"

namespace Nutmeg
{

MessageBox::MessageBox(QWidget *parent)
    : QMessageBox(parent)
{
    setModal(true);
}

} // namespace Nutmeg
