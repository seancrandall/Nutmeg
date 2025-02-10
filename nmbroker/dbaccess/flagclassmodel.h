#ifndef NUTMEG_FLAGCLASSMODEL_H
#define NUTMEG_FLAGCLASSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class flagClassModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit flagClassModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_FLAGCLASSMODEL_H
