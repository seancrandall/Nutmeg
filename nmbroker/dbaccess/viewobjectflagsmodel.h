#ifndef NUTMEG_VIEWOBJECTFLAGSMODEL_H
#define NUTMEG_VIEWOBJECTFLAGSMODEL_H

#include <QObject>
#include <QSqlRecord>
#include <QPersistentModelIndex>

#include "tablemodel.h"

namespace Nutmeg {

class viewObjectFlagsModel : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit viewObjectFlagsModel(QObject *parent = nullptr);
    
    static QSqlRecord fetchRecord(Key primaryKey);

    bool contains(Key key, const QString& camelCase) const;
    Key getIndex(Key objectId, const QString& camelCase) const;

private:
    QHash<QPair<Key, QString>, QPersistentModelIndex> mPairs;
    void initializePairs(void);
};

} // namespace Nutmeg

#endif // NUTMEG_VIEWOBJECTFLAGSMODEL_H
