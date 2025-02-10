#include "viewpatentmattersmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewPatentMattersModel::viewPatentMattersModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewPatentMatters");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewPatentMattersModel::record(Key primaryKey)
{
	//gViewPatentMatters is already globally defined. 
	return Nutmeg::record<viewPatentMattersModel>(primaryKey, gViewPatentMattersModel);
}

} // namespace Nutmeg
