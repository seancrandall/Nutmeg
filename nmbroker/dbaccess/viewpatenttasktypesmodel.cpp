#include "viewpatenttasktypesmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewPatentTaskTypesModel::viewPatentTaskTypesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewPatentTaskTypes");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewPatentTaskTypesModel::fetchRecord(Key primaryKey)
{
	//gViewPatentTaskTypes is already globally defined. 
	return Nutmeg::record<viewPatentTaskTypesModel>(primaryKey, gViewPatentTaskTypesModel);
}

} // namespace Nutmeg
