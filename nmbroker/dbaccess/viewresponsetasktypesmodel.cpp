#include "viewresponsetasktypesmodel.h"

namespace Nutmeg
{

viewResponseTaskTypesModel::viewResponseTaskTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewResponseTaskTypes");
    select();
}

} // namespace Nutmeg
