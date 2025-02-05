#include "workattorneysearchbox.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

WorkAttorneySearchBox::WorkAttorneySearchBox(QWidget *parent)
    : Nutmeg::PersonSearchBox(parent)
{
    Initialize();
}

WorkAttorneySearchBox::WorkAttorneySearchBox(Key initkey, QWidget *parent)
    : Nutmeg::PersonSearchBox(parent)
{
    Initialize();
    key = initkey;
}

void WorkAttorneySearchBox::Initialize()
{
    CheckModels();
    column = 3;
}

void WorkAttorneySearchBox::CheckModels()
{
    if(!gViewWorkAttorneysModel)
        gViewWorkAttorneysModel = std::make_unique<viewWorkAttorneysModel>();
    model = gViewWorkAttorneysModel.get();
}

} // namespace Nutmeg
