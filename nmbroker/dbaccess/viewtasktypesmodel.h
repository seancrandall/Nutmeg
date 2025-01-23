#ifndef NUTMEG_VIEWTASKTYPESMODEL_H
#define NUTMEG_VIEWTASKTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewTaskTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTASKTYPESMODEL_H
