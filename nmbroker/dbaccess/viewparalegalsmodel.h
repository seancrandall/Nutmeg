#ifndef NUTMEG_VIEWPARALEGALSMODEL_H
#define NUTMEG_VIEWPARALEGALSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewParalegalsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewParalegalsModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPARALEGALSMODEL_H
