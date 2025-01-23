#include "viewclientsmodel.h"

namespace Nutmeg
{

viewClientsModel::viewClientsModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewClients");
    select();
}

} // namespace Nutmeg
