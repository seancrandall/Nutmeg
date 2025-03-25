#ifndef NUTMEG_VIEWENTITIESMODEL_H
#define NUTMEG_VIEWENTITIESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewEntitiesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewEntitiesModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWENTITIESMODEL_H
