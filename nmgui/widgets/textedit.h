#ifndef NUTMEG_TEXTEDIT_H
#define NUTMEG_TEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

#include "nutmeg.h"

namespace Nutmeg
{

class TextEdit : public QTextEdit
{
    Q_OBJECT
  public:
    TextEdit(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_TEXTEDIT_H
