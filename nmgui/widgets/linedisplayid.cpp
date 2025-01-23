#include "linedisplayid.h"

namespace Nutmeg
{

LineDisplayId::LineDisplayId(QWidget *parent) : Nutmeg::LineEdit(parent)
{
    setReadOnly(true);
    id = 0;
}

LineDisplayId::LineDisplayId(Key displayKey, QWidget *parent) : Nutmeg::LineEdit(parent)
{
    setReadOnly(true);
    id = displayKey;
}

Key LineDisplayId::getId()
{
    QVariant var = text;
    return var.toUInt();
}

void LineDisplayId::setId(Key newid) { text = QString::number(newid); }

} // namespace Nutmeg
