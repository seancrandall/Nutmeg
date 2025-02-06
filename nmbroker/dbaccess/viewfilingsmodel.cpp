#include "viewfilingsmodel.h"

namespace Nutmeg {

viewFilingsModel::viewFilingsModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewFilings");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
