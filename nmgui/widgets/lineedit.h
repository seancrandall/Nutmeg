#ifndef NUTMEG_LINEEDIT_H
#define NUTMEG_LINEEDIT_H

#include <QLineEdit>
#include <QObject>
#include <QWidget>

#include "nutmeg.h"
#include "property.h"

namespace Nutmeg
{

class LineEdit : public QLineEdit
{
    Q_OBJECT
  public:
    LineEdit(QWidget *parent = nullptr);

    // Properties
    Property(getText, setText) QString text;

    const QString getText(void) { return QLineEdit::text(); }
};

} // namespace Nutmeg

#endif // NUTMEG_LINEEDIT_H
