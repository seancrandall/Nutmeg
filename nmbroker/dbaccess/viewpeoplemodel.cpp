#include "viewpeoplemodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewPeopleModel::viewPeopleModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewPeople");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewPeopleModel::fetchRecord(Key primaryKey)
{
	//gViewPeople is already globally defined. 
	return Nutmeg::record<viewPeopleModel>(primaryKey, gViewPeopleModel);
}

} // namespace Nutmeg
