#include "viewobjectflagsmodel.h"

namespace Nutmeg {

viewObjectFlagsModel::viewObjectFlagsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewObjectFlags");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
