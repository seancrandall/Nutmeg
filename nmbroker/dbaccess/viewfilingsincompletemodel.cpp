#include "viewfilingsincompletemodel.h"

namespace Nutmeg
{

viewFilingsIncompleteModel::viewFilingsIncompleteModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewFilingsIncomplete");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
