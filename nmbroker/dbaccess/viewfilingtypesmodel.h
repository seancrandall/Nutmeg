#ifndef NUTMEG_VIEWFILINGTYPESMODEL_H
#define NUTMEG_VIEWFILINGTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewFilingTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewFilingTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWFILINGTYPESMODEL_H
