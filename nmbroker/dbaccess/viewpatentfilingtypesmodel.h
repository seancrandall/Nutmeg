#ifndef NUTMEG_VIEWPATENTFILINGTYPESMODEL_H
#define NUTMEG_VIEWPATENTFILINGTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewPatentFilingTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewPatentFilingTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPATENTFILINGTYPESMODEL_H
