#ifndef NUTMEG_VIEWTRADEMARKFILINGTYPESMODEL_H
#define NUTMEG_VIEWTRADEMARKFILINGTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewTrademarkFilingTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewTrademarkFilingTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTRADEMARKFILINGTYPESMODEL_H
