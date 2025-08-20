#ifndef NUTMEG_RESPONSESDASHBOARDCOMPLETE_H
#define NUTMEG_RESPONSESDASHBOARDCOMPLETE_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class ResponsesDashboardComplete : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit ResponsesDashboardComplete(QObject *parent = nullptr);

    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSESDASHBOARDCOMPLETE_H
