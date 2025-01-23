#ifndef NUTMEG_VIEWENTITIESMODEL_H
#define NUTMEG_VIEWENTITIESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewEntitiesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewEntitiesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWENTITIESMODEL_H
