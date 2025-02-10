#ifndef NUTMEG_TAGMODEL_H
#define NUTMEG_TAGMODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"
#include "property.h"

namespace Nutmeg {

class tagModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit tagModel(QObject *parent = nullptr);

    ReadOnlyProperty(getTagidByText) Key tagId[];
    
    static QSqlRecord record(Key primaryKey);
    Key getTagidByText(const QString& text) const;

    QHash<QString, Key> mTagsByText;

private:
    void PopulateTagsByText(void);
};

} // namespace Nutmeg

#endif // NUTMEG_TAGMODEL_H
