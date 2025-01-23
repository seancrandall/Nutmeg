#include "criticalbox.h"

namespace Nutmeg
{

CriticalBox::CriticalBox(QWidget *parent) : Nutmeg::MessageBox(parent) { icon = QMessageBox::Critical; }

} // namespace Nutmeg
