#include "appointmenttypemodel.h"

namespace Nutmeg {

appointmentTypeModel::appointmentTypeModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("appointmentType");
    select();
}

} // namespace Nutmeg
