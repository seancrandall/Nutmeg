#include "viewpatentexaminersmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewPatentExaminersModel::viewPatentExaminersModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewPatentExaminers");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord viewPatentExaminersModel::record(Key primaryKey)
{
	//gViewPatentExaminers is already globally defined. 
	return Nutmeg::record<viewPatentExaminersModel>(primaryKey, gViewPatentExaminersModel);
}

} // namespace Nutmeg
