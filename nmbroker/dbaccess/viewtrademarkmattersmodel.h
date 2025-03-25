#ifndef NUTMEG_VIEWTRADEMARKMATTERSMODEL_H
#define NUTMEG_VIEWTRADEMARKMATTERSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewTrademarkMattersModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewTrademarkMattersModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWTRADEMARKMATTERSMODEL_H
