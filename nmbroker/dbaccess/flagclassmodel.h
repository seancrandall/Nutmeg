#ifndef NUTMEG_FLAGCLASSMODEL_H
#define NUTMEG_FLAGCLASSMODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class flagClassModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit flagClassModel(QObject *parent = nullptr);

    ReadOnlyProperty(getFlagByCamelCase) Key camelCase[];

    Key getFlagByCamelCase(QString cc);

private:
    QHash<QString, Key> mFlagsByCamelCase;
};

} // namespace Nutmeg

#endif // NUTMEG_FLAGCLASSMODEL_H
