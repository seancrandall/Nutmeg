#ifndef NUTMEG_VIEWTASKSMODEL_H
#define NUTMEG_VIEWTASKSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewTasksModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewTasksModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTASKSMODEL_H
