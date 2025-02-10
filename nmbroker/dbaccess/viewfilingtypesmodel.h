#ifndef NUTMEG_VIEWFILINGTYPESMODEL_H
#define NUTMEG_VIEWFILINGTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewFilingTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewFilingTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWFILINGTYPESMODEL_H
