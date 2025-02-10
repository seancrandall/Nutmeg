#include "viewworkattorneysmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewWorkAttorneysModel::viewWorkAttorneysModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewWorkAttorneys");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewWorkAttorneysModel::record(Key primaryKey)
{
	//gViewWorkAttorneys is already globally defined. 
	return Nutmeg::record<viewWorkAttorneysModel>(primaryKey, gViewWorkAttorneysModel);
}

} // namespace Nutmeg
