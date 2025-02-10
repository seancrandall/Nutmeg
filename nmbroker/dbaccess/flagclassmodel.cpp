#include "flagclassmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

flagClassModel::flagClassModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("flagClass");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord flagClassModel::record(Key primaryKey)
{
	//gFlagClass is already globally defined. 
	return Nutmeg::record<flagClassModel>(primaryKey, gFlagClassModel);
}

} // namespace Nutmeg
