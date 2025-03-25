#ifndef NUTMEG_VIEWCOPYRIGHTTASKTYPESMODEL_H
#define NUTMEG_VIEWCOPYRIGHTTASKTYPESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewCopyrightTaskTypesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewCopyrightTaskTypesModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWCOPYRIGHTTASKTYPESMODEL_H
