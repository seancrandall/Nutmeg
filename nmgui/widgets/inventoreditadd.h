#ifndef NUTMEG_INVENTOREDITADD_H
#define NUTMEG_INVENTOREDITADD_H

#include "comboeditadd.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class InventorEditAdd : public Nutmeg::ComboEditAdd
{
    Q_OBJECT
  public:
    InventorEditAdd(QWidget *parent = nullptr);
    InventorEditAdd(QString label, QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_INVENTOREDITADD_H
