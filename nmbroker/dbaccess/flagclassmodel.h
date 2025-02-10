#ifndef NUTMEG_FLAGCLASSMODEL_H
#define NUTMEG_FLAGCLASSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"
#include "structures.h"
#include "property.h"

namespace Nutmeg {

class flagClassModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit flagClassModel(QObject *parent = nullptr);

    ReadOnlyProperty(getFlagByCamelCase) Key camelCase[];
    
    static QSqlRecord record(Key primaryKey);
    static const FlagClassData record(const QString& camelCase);

    Key getFlagByCamelCase(const QString& cc) const;

private:
    QHash<QString, Key> mFlagsByCamelCase;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // NUTMEG_FLAGCLASSMODEL_H
