#include "viewobjecttagsmodel.h"

namespace Nutmeg {

viewObjectTagsModel::viewObjectTagsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewObjectTags");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
