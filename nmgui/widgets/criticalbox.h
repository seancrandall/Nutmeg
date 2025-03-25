#ifndef NUTMEG_CRITICALBOX_H
#define NUTMEG_CRITICALBOX_H

#include "messagebox.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class CriticalBox : public Nutmeg::MessageBox
{
    Q_OBJECT
  public:
    CriticalBox(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_CRITICALBOX_H
