#ifndef NUTMEG_VIEWPATENTEXAMINERSMODEL_H
#define NUTMEG_VIEWPATENTEXAMINERSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewPatentExaminersModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewPatentExaminersModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPATENTEXAMINERSMODEL_H
