#ifndef NUTMEG_ENTITIESCOMPLETER_H
#define NUTMEG_ENTITIESCOMPLETER_H

#include <QObject>

#include "dbaccess/viewentitiesmodel.h"
#include "completer.h"

namespace Nutmeg
{

class EntitiesCompleter : public Nutmeg::Completer
{
    Q_OBJECT
  public:
    explicit EntitiesCompleter(QObject *parent = nullptr);

  private:
    viewEntitiesModel *mod;
};

} // namespace Nutmeg

#endif // NUTMEG_ENTITIESCOMPLETER_H
