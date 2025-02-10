#ifndef NUTMEG_TAGMODEL_H
#define NUTMEG_TAGMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class tagModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit tagModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_TAGMODEL_H
