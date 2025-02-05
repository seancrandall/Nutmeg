#include "viewclientsmodel.h"

namespace Nutmeg
{

viewClientsModel::viewClientsModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewClients");
    select();
    mIsLoaded = true;
    IndexLocations();
}

} // namespace Nutmeg
