#include "viewfilingtypesmodel.h"

namespace Nutmeg
{

viewFilingTypesModel::viewFilingTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewFilingTypes");
    select();
}

} // namespace Nutmeg
