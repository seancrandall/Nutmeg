#ifndef NUTMEG_INVENTORCOMPLETER_H
#define NUTMEG_INVENTORCOMPLETER_H

#include <QObject>
#include <QWidget>

#include "completer.h"
#include "dbaccess/viewinventorsmodel.h"

namespace Nutmeg
{

class InventorCompleter : public Nutmeg::Completer
{
    Q_OBJECT
  public:
    explicit InventorCompleter(QObject *parent = nullptr);

  private:
    viewInventorsModel *mod;
};

} // namespace Nutmeg

#endif // NUTMEG_INVENTORCOMPLETER_H
