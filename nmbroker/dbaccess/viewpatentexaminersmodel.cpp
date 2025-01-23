#include "viewpatentexaminersmodel.h"

namespace Nutmeg
{

viewPatentExaminersModel::viewPatentExaminersModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewPatentExaminers");
    select();
}

} // namespace Nutmeg
