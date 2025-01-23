#ifndef NUTMEG_VIEWFILINGTASKTYPESMODEL_H
#define NUTMEG_VIEWFILINGTASKTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewFilingTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewFilingTaskTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWFILINGTASKTYPESMODEL_H
