#include "viewfilingtasktypesmodel.h"

namespace Nutmeg
{

viewFilingTaskTypesModel::viewFilingTaskTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewFilingTaskTypes");
    select();
}

} // namespace Nutmeg
