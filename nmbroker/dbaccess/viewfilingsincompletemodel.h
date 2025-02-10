#ifndef NUTMEG_VIEWFILINGSINCOMPLETEMODEL_H
#define NUTMEG_VIEWFILINGSINCOMPLETEMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewFilingsIncompleteModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewFilingsIncompleteModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWFILINGSINCOMPLETEMODEL_H
