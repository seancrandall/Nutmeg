#include "viewobjecttagsmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

viewObjectTagsModel::viewObjectTagsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewObjectTags");
    if(select())
    {
        IndexLocations();
    }
}

QSqlRecord viewObjectTagsModel::fetchRecord(Key primaryKey)
{
	//gViewObjectTags is already globally defined. 
	return Nutmeg::record<viewObjectTagsModel>(primaryKey, gViewObjectTagsModel);
}

} // namespace Nutmeg
