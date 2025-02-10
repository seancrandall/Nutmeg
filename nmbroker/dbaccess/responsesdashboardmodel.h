#ifndef NUTMEG_RESPONSESDASHBOARDMODEL_H
#define NUTMEG_RESPONSESDASHBOARDMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class responsesDashboardModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit responsesDashboardModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSESDASHBOARDMODEL_H
