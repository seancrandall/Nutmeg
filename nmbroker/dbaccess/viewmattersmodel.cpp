#include "viewmattersmodel.h"

namespace Nutmeg
{

viewMattersModel::viewMattersModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewMatters");
    select();
}

} // namespace Nutmeg
