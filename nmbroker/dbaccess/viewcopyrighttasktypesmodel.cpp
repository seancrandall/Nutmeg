#include "viewcopyrighttasktypesmodel.h"

namespace Nutmeg
{

viewCopyrightTaskTypesModel::viewCopyrightTaskTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewCopyrightTaskTypes");
    select();
}

} // namespace Nutmeg
