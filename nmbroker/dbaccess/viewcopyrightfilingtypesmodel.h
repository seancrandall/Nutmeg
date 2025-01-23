#ifndef NUTMEG_VIEWCOPYRIGHTFILINGTYPESMODEL_H
#define NUTMEG_VIEWCOPYRIGHTFILINGTYPESMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class viewCopyrightFilingTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit viewCopyrightFilingTypesModel(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWCOPYRIGHTFILINGTYPESMODEL_H
