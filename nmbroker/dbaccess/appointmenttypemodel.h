#ifndef NUTMEG_APPOINTMENTTYPEMODEL_H
#define NUTMEG_APPOINTMENTTYPEMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class appointmentTypeModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit appointmentTypeModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_APPOINTMENTTYPEMODEL_H
