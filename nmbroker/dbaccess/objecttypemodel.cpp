#include <QSqlField>
#include "objecttypemodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

objectTypeModel::objectTypeModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("objectType");
    if(select()){
        IndexLocations();
        for(int i = 0; i < rowCount(); i++){
            QString name = record(i).field("TypeName").value().toString();
            Key id = record(i).field("ObjectTypeId").value().toUInt();
            mTypeNames[name] = id;
        }
    }
}

QSqlRecord objectTypeModel::fetchRecord(Key primaryKey)
{
	//gObjectType is already globally defined. 
	return Nutmeg::record<objectTypeModel>(primaryKey, gObjectTypeModel);
}

Key objectTypeModel::getIdFromName(const QString &typeName) const
{
    return mTypeNames[typeName];
}

} // namespace Nutmeg
