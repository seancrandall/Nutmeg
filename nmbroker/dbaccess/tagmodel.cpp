#include "tagmodel.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

tagModel::tagModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("tag");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord tagModel::record(Key primaryKey)
{
	//gTag is already globally defined. 
	return Nutmeg::record<tagModel>(primaryKey, gTagModel);
}

} // namespace Nutmeg
