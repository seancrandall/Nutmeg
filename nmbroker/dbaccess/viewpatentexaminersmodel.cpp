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
        IndexLocations();
    }
}

QSqlRecord viewPatentExaminersModel::fetchRecord(Key primaryKey)
{
	//gViewPatentExaminers is already globally defined. 
	return Nutmeg::record<viewPatentExaminersModel>(primaryKey, gViewPatentExaminersModel);
}

} // namespace Nutmeg
