#include "label.h"

namespace Nutmeg
{

Label::Label(QWidget *parent) : QLabel(parent) {}

Label::Label(const QString &text, QWidget *parent) : QLabel(text, parent) {}

} // namespace Nutmeg
