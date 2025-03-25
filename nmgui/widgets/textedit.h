#ifndef NUTMEG_TEXTEDIT_H
#define NUTMEG_TEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

#include "nutmeg.h"
#include "property.h"

namespace Nutmeg
{

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = nullptr);

    Property(getText, setText) QString text;

    const QString getText(void) const;
    void setText(const QString& newtext);
};

} // namespace Nutmeg

#endif // NUTMEG_TEXTEDIT_H
