#ifndef NUTMEG_VIEWINVENTORSMODEL_H
#define NUTMEG_VIEWINVENTORSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewInventorsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewInventorsModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWINVENTORSMODEL_H
