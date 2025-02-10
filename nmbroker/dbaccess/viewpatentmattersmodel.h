#ifndef NUTMEG_VIEWPATENTMATTERSMODEL_H
#define NUTMEG_VIEWPATENTMATTERSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewPatentMattersModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewPatentMattersModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPATENTMATTERSMODEL_H
