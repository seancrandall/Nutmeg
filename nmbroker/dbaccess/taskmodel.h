#ifndef NUTMEG_TASKMODEL_H
#define NUTMEG_TASKMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class taskModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit taskModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_TASKMODEL_H
