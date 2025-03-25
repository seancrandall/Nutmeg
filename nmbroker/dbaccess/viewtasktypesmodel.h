#ifndef NUTMEG_VIEWTASKTYPESMODEL_H
#define NUTMEG_VIEWTASKTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewTaskTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTASKTYPESMODEL_H
