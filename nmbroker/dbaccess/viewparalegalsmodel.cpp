#include "viewparalegalsmodel.h"

namespace Nutmeg
{

viewParalegalsModel::viewParalegalsModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewParalegals");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
