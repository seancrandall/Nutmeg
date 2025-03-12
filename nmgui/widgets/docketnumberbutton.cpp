#include "docketnumberbutton.h"

#include "windows/matterdialog.h"
#include "windows/patentmatterdialog.h"

namespace Nutmeg
{

DocketNumberButton::DocketNumberButton(std::shared_ptr<Matter> matter, QWidget *parent)
    : Nutmeg::PushButton(parent)
    , mMatter(matter)
{
    setText(matter->AttorneyDocketNumber);
    // setFixedWidth(200);

    QObject::connect(this,      &QPushButton::clicked,
                     this,       &DocketNumberButton::clicked);
}

void DocketNumberButton::clicked()
{
    switch(mMatter->fkObjectType){
    case ObjectType::patentMatter :
        break;

    }
}

} // namespace Nutmeg
