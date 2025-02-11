#ifndef NUTMEG_VIEWPATENTTASKTYPESMODEL_H
#define NUTMEG_VIEWPATENTTASKTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewPatentTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewPatentTaskTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPATENTTASKTYPESMODEL_H
