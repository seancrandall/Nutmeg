#ifndef NUTMEG_OBJECTMODEL_H
#define NUTMEG_OBJECTMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class objectModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit objectModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_OBJECTMODEL_H
