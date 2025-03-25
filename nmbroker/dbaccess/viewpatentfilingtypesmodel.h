#ifndef NUTMEG_VIEWPATENTFILINGTYPESMODEL_H
#define NUTMEG_VIEWPATENTFILINGTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewPatentFilingTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewPatentFilingTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPATENTFILINGTYPESMODEL_H
