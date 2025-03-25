#ifndef NUTMEG_VIEWJURISDICTIONSMODEL_H
#define NUTMEG_VIEWJURISDICTIONSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewJurisdictionsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewJurisdictionsModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWJURISDICTIONSMODEL_H
