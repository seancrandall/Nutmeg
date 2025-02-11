#ifndef NUTMEG_OBJECTTYPEMODEL_H
#define NUTMEG_OBJECTTYPEMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"
#include "property.h"

namespace Nutmeg {

class objectTypeModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit objectTypeModel(QObject *parent = nullptr);

    ReadOnlyProperty(getIdFromName) Key typeKey[];
    
    static QSqlRecord fetchRecord(Key primaryKey);

    Key getIdFromName(const QString& typeName) const;

private:
    QHash<QString, Key> mTypeNames;
};

} // namespace Nutmeg

#endif // NUTMEG_OBJECTTYPEMODEL_H
