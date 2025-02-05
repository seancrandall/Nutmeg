#include "viewcopyrightfilingtypesmodel.h"

namespace Nutmeg
{

viewCopyrightFilingTypesModel::viewCopyrightFilingTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewCopyrightFilingTypes");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
