#ifndef NUTMEG_VIEWTASKCLASSMODEL_H
#define NUTMEG_VIEWTASKCLASSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewTaskClassModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewTaskClassModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTASKCLASSMODEL_H
