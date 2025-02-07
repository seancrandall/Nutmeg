#include "tagmodel.h"

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

} // namespace Nutmeg
