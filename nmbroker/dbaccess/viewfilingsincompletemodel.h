#ifndef NUTMEG_VIEWFILINGSINCOMPLETEMODEL_H
#define NUTMEG_VIEWFILINGSINCOMPLETEMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewFilingsIncompleteModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewFilingsIncompleteModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWFILINGSINCOMPLETEMODEL_H
