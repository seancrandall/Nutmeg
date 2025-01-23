#include "informationbox.h"

namespace Nutmeg
{

InformationBox::InformationBox(QWidget *parent) : Nutmeg::MessageBox(parent) { icon = QMessageBox::Information; }

} // namespace Nutmeg
