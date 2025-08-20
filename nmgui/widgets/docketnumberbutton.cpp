#include "docketnumberbutton.h"

#include "objects/task.h"
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

DocketNumberButton::DocketNumberButton(const responsesDashboardEntry &entry, QWidget *parent)
    : Nutmeg::PushButton(parent)
    , mMatter([&entry]() {
          Task t(entry.getTaskId());
          return std::make_shared<Matter>(t.fkMatter);
      }())
{
    setText(entry.getAttorneyDocketNumber());
    QObject::connect(this,      &QPushButton::clicked,
                     this,       &DocketNumberButton::clicked);
}

void DocketNumberButton::clicked()
{
    switch(mMatter->fkObjectType){
    case ObjectType::patentMatter:
    {
        std::shared_ptr<PatentMatter> pMatter = std::make_shared<PatentMatter>(mMatter->MatterId);
        PatentMatterDialog diag(pMatter);
        diag.setModal(true);
        diag.exec();
        break;
    }
    case ObjectType::trademarkMatter:
    case ObjectType::copyrightMatter:
    case ObjectType::generalMatter:
    {
        MatterDialog diag(mMatter);
        diag.setModal(true);
        diag.exec();
    }
    default:
        //do nothing
        break;
    }
}

} // namespace Nutmeg
