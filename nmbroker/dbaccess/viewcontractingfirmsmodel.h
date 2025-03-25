#ifndef NUTMEG_VIEWCONTRACTINGFIRMSMODEL_H
#define NUTMEG_VIEWCONTRACTINGFIRMSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewContractingFirmsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewContractingFirmsModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWCONTRACTINGFIRMSMODEL_H
