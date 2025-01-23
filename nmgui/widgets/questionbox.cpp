#include "questionbox.h"

namespace Nutmeg
{

QuestionBox::QuestionBox(QWidget *parent) : Nutmeg::MessageBox(parent) { icon = QMessageBox::Question; }

} // namespace Nutmeg
