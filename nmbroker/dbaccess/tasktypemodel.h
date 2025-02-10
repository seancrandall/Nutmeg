#ifndef NUTMEG_TASKTYPEMODEL_H
#define NUTMEG_TASKTYPEMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class taskTypeModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit taskTypeModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_TASKTYPEMODEL_H
