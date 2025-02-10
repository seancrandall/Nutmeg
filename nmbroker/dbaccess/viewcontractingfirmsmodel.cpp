#include "viewcontractingfirmsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewContractingFirmsModel::viewContractingFirmsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewContractingFirms");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewContractingFirmsModel::record(Key primaryKey)
{
	//gViewContractingFirms is already globally defined. 
	return Nutmeg::record<viewContractingFirmsModel>(primaryKey, gViewContractingFirmsModel);
}

} // namespace Nutmeg
