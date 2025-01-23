#include "viewfilingsincompletemodel.h"

namespace Nutmeg
{

viewFilingsIncompleteModel::viewFilingsIncompleteModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewFilingsIncomplete");
    select();
}

} // namespace Nutmeg
