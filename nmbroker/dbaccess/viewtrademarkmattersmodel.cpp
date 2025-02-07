#include "viewtrademarkmattersmodel.h"

namespace Nutmeg {

viewTrademarkMattersModel::viewTrademarkMattersModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewTrademarkMatters");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
