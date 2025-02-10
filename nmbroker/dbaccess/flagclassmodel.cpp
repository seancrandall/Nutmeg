#include "flagclassmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

flagClassModel::flagClassModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    Initialize();
}

QSqlRecord flagClassModel::record(Key primaryKey)
{
	//gFlagClass is already globally defined. 
	return Nutmeg::record<flagClassModel>(primaryKey, gFlagClassModel);
}

Key flagClassModel::getFlagByCamelCase(const QString &cc) const
{
    if(!mFlagsByCamelCase.contains(cc)){
        return UINT32_MAX;
    }

    return mFlagsByCamelCase[cc];
}

void flagClassModel::Initialize()
{
    setTable("flagClass");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }

    mFlagsByCamelCase = QHash<QString, Key>();
    for(int i = 0; i < rowCount(); i++){
        QString camelCase = record(i).value("camelCase").toString();
        Key flagClassId = record(i).value("FlagClassId").toUInt();
        mFlagsByCamelCase[camelCase] = flagClassId;
    }
}

} // namespace Nutmeg
