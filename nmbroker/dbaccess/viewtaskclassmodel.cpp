#include "viewtaskclassmodel.h"

namespace Nutmeg
{

viewTaskClassModel::viewTaskClassModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewTaskClass");
    select();
}

} // namespace Nutmeg