#ifndef NUTMEG_VIEWRESPONSETASKTYPESMODEL_H
#define NUTMEG_VIEWRESPONSETASKTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewResponseTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewResponseTaskTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWRESPONSETASKTYPESMODEL_H
