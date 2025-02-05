#include "viewtrademarktasktypesmodel.h"

namespace Nutmeg
{

viewTrademarkTaskTypesModel::viewTrademarkTaskTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewTrademarkTaskTypes");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
