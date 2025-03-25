#include "warningbox.h"

namespace Nutmeg
{

WarningBox::WarningBox(const QString& warningText, QWidget *parent) : Nutmeg::MessageBox(parent)
{
    icon = QMessageBox::Warning;
    if(warningText != QString())
        setText(warningText);
}

} // namespace Nutmeg
