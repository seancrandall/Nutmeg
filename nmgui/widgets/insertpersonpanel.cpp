#include "insertpersonpanel.h"

namespace Nutmeg
{

InsertPersonPanel::InsertPersonPanel(QWidget *parent) : QWidget{parent}
{
    box->addLayout(lFirstName);
    box->addLayout(lLastName);

    setLayout(box);
}

} // namespace Nutmeg
