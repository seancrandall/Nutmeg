#include "warningbox.h"

namespace Nutmeg
{

WarningBox::WarningBox(QWidget *parent) : Nutmeg::MessageBox(parent) { icon = QMessageBox::Warning; }

} // namespace Nutmeg
