#include "viewobjectflagsmodel.h"
#include "models.h"
#include "record.h"
#include "logger.h"

#include <QSqlField>

namespace Nutmeg {

viewObjectFlagsModel::viewObjectFlagsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewObjectFlags");
    if(select())
    {
        TableModel::IndexLocations();
        initializePairs();
    } else {
        Logger::LogMessage("Failed initializing viewObjectFlags class.");
    }
}

QSqlRecord viewObjectFlagsModel::fetchRecord(Key primaryKey)
{
	//gViewObjectFlags is already globally defined. 
	return Nutmeg::record<viewObjectFlagsModel>(primaryKey, gViewObjectFlagsModel);
}

bool viewObjectFlagsModel::contains(Key key, const QString &camelCase) const
{
    return mPairs.contains(QPair<Key, QString>(key, camelCase));
}

Key viewObjectFlagsModel::getIndex(Key objectId, const QString &camelCase) const
{
    if(!contains(objectId, camelCase))
        return 0;

    QPersistentModelIndex idx = mPairs[QPair<Key, QString>(objectId, camelCase)];
    return idx.data().toUInt();
}

void viewObjectFlagsModel::initializePairs()
{
    mPairs.clear();
    for(int i = 0; i < rowCount(); i++){
        QSqlRecord rec = record(i);
        if (rec.contains("ObjectId") && rec.contains("CamelCase")) {
            Key key = rec.field("ObjectId").value().toUInt();
            QString cc = rec.field("CamelCase").value().toString();
            QPersistentModelIndex idx(index(i, 0)); //Get the index of the primary key for this row
            mPairs.insert(QPair<Key, QString>(key, cc), idx);
        } else {

        }
    }
}
} // namespace Nutmeg
