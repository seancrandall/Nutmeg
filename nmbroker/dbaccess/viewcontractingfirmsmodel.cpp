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
        IndexLocations();
    }
}

QSqlRecord viewContractingFirmsModel::fetchRecord(Key primaryKey)
{
	//gViewContractingFirms is already globally defined. 
	return Nutmeg::record<viewContractingFirmsModel>(primaryKey, gViewContractingFirmsModel);
}

} // namespace Nutmeg
