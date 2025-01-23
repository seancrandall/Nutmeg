#include "viewtrademarktasktypesmodel.h"

namespace Nutmeg
{

viewTrademarkTaskTypesModel::viewTrademarkTaskTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewTrademarkTaskTypes");
    select();
}

} // namespace Nutmeg
