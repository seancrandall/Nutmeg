#ifndef NUTMEG_VIEWTRADEMARKFILINGTYPESMODEL_H
#define NUTMEG_VIEWTRADEMARKFILINGTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewTrademarkFilingTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewTrademarkFilingTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTRADEMARKFILINGTYPESMODEL_H
