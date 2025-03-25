#ifndef NUTMEG_VIEWMATTERSMODEL_H
#define NUTMEG_VIEWMATTERSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewMattersModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewMattersModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWMATTERSMODEL_H
