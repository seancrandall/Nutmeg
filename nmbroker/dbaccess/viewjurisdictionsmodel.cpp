#include "viewjurisdictionsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewJurisdictionsModel::viewJurisdictionsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewJurisdictions");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewJurisdictionsModel::record(Key primaryKey)
{
	//gViewJurisdictions is already globally defined. 
	return Nutmeg::record<viewJurisdictionsModel>(primaryKey, gViewJurisdictionsModel);
}

} // namespace Nutmeg
