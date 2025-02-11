#ifndef NUTMEG_VIEWUPCOMINGAPPOINTMENTSMODEL_H
#define NUTMEG_VIEWUPCOMINGAPPOINTMENTSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewUpcomingAppointmentsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewUpcomingAppointmentsModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWUPCOMINGAPPOINTMENTSMODEL_H
