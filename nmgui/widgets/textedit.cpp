#include "textedit.h"

namespace Nutmeg
{

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent)
{
    NUTMEGFONT;
}

const QString TextEdit::getText() const
{
    return QTextEdit::toPlainText();
}

void TextEdit::setText(const QString &newtext)
{
    QTextEdit::setText(newtext);
}

} // namespace Nutmeg
