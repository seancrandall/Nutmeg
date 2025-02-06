#include "deadlinemodel.h"

namespace Nutmeg {

deadlineModel::deadlineModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("deadline");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
