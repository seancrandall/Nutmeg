#ifndef NUTMEG_QUESTIONBOX_H
#define NUTMEG_QUESTIONBOX_H

#include "messagebox.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class QuestionBox : public Nutmeg::MessageBox
{
    Q_OBJECT
  public:
    QuestionBox(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_QUESTIONBOX_H
