#ifndef NUTMEG_TASKTYPEMODEL_H
#define NUTMEG_TASKTYPEMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class taskTypeModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit taskTypeModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_TASKTYPEMODEL_H
