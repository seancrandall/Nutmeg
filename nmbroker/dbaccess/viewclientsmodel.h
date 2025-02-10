#ifndef NUTMEG_VIEWCLIENTSMODEL_H
#define NUTMEG_VIEWCLIENTSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewClientsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewClientsModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWCLIENTSMODEL_H
