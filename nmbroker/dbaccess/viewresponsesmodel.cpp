#include "viewresponsesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewResponsesModel::viewResponsesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewResponses");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewResponsesModel::record(Key primaryKey)
{
	//gViewResponses is already globally defined. 
	return Nutmeg::record<viewResponsesModel>(primaryKey, gViewResponsesModel);
}

} // namespace Nutmeg
