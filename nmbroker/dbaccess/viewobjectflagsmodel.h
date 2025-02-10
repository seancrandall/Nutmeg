#ifndef NUTMEG_VIEWOBJECTFLAGSMODEL_H
#define NUTMEG_VIEWOBJECTFLAGSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewObjectFlagsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewObjectFlagsModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWOBJECTFLAGSMODEL_H
