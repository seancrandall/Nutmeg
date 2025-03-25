#ifndef NUTMEG_VIEWRESPONSESMODEL_H
#define NUTMEG_VIEWRESPONSESMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewResponsesModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewResponsesModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWRESPONSESMODEL_H
