#ifndef NUTMEG_WARNINGBOX_H
#define NUTMEG_WARNINGBOX_H

#include "messagebox.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class WarningBox : public Nutmeg::MessageBox
{
    Q_OBJECT
  public:
    WarningBox(const QString& warningText = QString(), QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_WARNINGBOX_H
