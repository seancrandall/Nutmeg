#include "viewresponsetasktypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewResponseTaskTypesModel::viewResponseTaskTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewResponseTaskTypes");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewResponseTaskTypesModel::record(Key primaryKey)
{
	//gViewResponseTaskTypes is already globally defined. 
	return Nutmeg::record<viewResponseTaskTypesModel>(primaryKey, gViewResponseTaskTypesModel);
}

} // namespace Nutmeg
