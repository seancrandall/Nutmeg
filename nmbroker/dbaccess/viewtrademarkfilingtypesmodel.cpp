#include "viewtrademarkfilingtypesmodel.h"

namespace Nutmeg
{

viewTrademarkFilingTypesModel::viewTrademarkFilingTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewTrademarkFilingTypes");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
