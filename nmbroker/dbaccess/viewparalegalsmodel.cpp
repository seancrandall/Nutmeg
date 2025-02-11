#include "viewparalegalsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewParalegalsModel::viewParalegalsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewParalegals");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewParalegalsModel::fetchRecord(Key primaryKey)
{
	//gViewParalegals is already globally defined. 
	return Nutmeg::record<viewParalegalsModel>(primaryKey, gViewParalegalsModel);
}

} // namespace Nutmeg
