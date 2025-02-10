#ifndef NUTMEG_VIEWCOPYRIGHTFILINGTYPESMODEL_H
#define NUTMEG_VIEWCOPYRIGHTFILINGTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewCopyrightFilingTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewCopyrightFilingTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWCOPYRIGHTFILINGTYPESMODEL_H
