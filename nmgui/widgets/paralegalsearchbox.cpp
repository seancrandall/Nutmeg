#include "paralegalsearchbox.h"

namespace Nutmeg {

ParalegalSearchBox::ParalegalSearchBox(QWidget *parent)
    : PersonSearchBox(parent)
{
    model = paraModel;
    Initialize();
}

ParalegalSearchBox::ParalegalSearchBox(Key inikey, QWidget *parent)
    : PersonSearchBox(parent)
{
    model = paraModel;
    key = inikey;
    Initialize();
}

void ParalegalSearchBox::Initialize()
{
    column = 9;
}

} // namespace Nutmeg


