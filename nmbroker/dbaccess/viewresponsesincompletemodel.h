#ifndef NUTMEG_VIEWRESPONSESINCOMPLETEMODEL_H
#define NUTMEG_VIEWRESPONSESINCOMPLETEMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewResponsesIncompleteModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewResponsesIncompleteModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWRESPONSESINCOMPLETEMODEL_H
