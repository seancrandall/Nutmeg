#ifndef NUTMEG_VIEWFILINGTASKTYPESMODEL_H
#define NUTMEG_VIEWFILINGTASKTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewFilingTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewFilingTaskTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWFILINGTASKTYPESMODEL_H
