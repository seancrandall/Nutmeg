#include "viewpatenttasktypesmodel.h"

namespace Nutmeg
{

viewPatentTaskTypesModel::viewPatentTaskTypesModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewPatentTaskTypes");
    select();
}

} // namespace Nutmeg
