#include "viewresponsesmodel.h"

namespace Nutmeg {

viewResponsesModel::viewResponsesModel(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("viewResponses");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

} // namespace Nutmeg
