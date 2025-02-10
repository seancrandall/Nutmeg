#ifndef NUTMEG_VIEWTRADEMARKTASKTYPESMODEL_H
#define NUTMEG_VIEWTRADEMARKTASKTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewTrademarkTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewTrademarkTaskTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTRADEMARKTASKTYPESMODEL_H
