#ifndef NUTMEG_VIEWCLIENTSMODEL_H
#define NUTMEG_VIEWCLIENTSMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewClientsModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewClientsModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWCLIENTSMODEL_H
