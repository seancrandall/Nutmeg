#include "viewmattersmodel.h"

namespace Nutmeg
{

viewMattersModel::viewMattersModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewMatters");
    if(select()){
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
