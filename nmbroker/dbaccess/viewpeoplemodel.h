#ifndef NUTMEG_VIEWPEOPLEMODEL_H
#define NUTMEG_VIEWPEOPLEMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class viewPeopleModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewPeopleModel(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWPEOPLEMODEL_H
