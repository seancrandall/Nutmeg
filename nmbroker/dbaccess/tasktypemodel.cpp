#include "tasktypemodel.h"

namespace Nutmeg
{

taskTypeModel::taskTypeModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("taskType");
    select();
}

} // namespace Nutmeg
