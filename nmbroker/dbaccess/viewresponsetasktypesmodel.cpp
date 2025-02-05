#include "viewresponsetasktypesmodel.h"

namespace Nutmeg
{

viewResponseTaskTypesModel::viewResponseTaskTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewResponseTaskTypes");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
