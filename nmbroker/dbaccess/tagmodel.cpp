#include "tagmodel.h"
#include "models.h"
#include "qsqlfield.h"
#include "record.h"

namespace Nutmeg {

tagModel::tagModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("tag");
    if(select())
    {
        IndexLocations();
    }

    PopulateTagsByText();
}

QSqlRecord tagModel::fetchRecord(Key primaryKey)
{
	//gTag is already globally defined. 
	return Nutmeg::record<tagModel>(primaryKey, gTagModel);
}

Key tagModel::getTagidByText(const QString &text) const
{
    return mTagsByText[text];
}

void tagModel::PopulateTagsByText()
{
    for(int i=0; i < rowCount(); i++){
        QSqlRecord rec = record(i);
        const QString text = rec.field("TagText").value().toString();
        mTagsByText[text] = rec.field("TagId").value().toUInt();
    }
}

} // namespace Nutmeg
