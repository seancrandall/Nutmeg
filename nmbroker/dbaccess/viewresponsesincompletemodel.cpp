#include "viewresponsesincompletemodel.h"

namespace Nutmeg
{

viewResponsesIncompleteModel::viewResponsesIncompleteModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewResponsesIncomplete");
    select();
}

} // namespace Nutmeg
