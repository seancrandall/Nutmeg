#include "workattorneysearchbox.h"

namespace Nutmeg
{

WorkAttorneySearchBox::WorkAttorneySearchBox(QWidget *parent)
    : Nutmeg::PersonSearchBox(parent)
{
    model = attyModel;
}

WorkAttorneySearchBox::WorkAttorneySearchBox(Key initkey, QWidget *parent)
    : Nutmeg::PersonSearchBox(parent)
{
    model = attyModel;
    key = initkey;
}

void WorkAttorneySearchBox::Initialize()
{
    column = 3;
}

} // namespace Nutmeg
