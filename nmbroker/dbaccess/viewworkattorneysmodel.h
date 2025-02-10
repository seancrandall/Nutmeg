#ifndef NUTMEG_VIEWWORKATTORNEYSMODEL_H
#define NUTMEG_VIEWWORKATTORNEYSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewWorkAttorneysModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewWorkAttorneysModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWWORKATTORNEYSMODEL_H
