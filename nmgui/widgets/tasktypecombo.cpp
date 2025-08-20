#include "tasktypecombo.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

TaskTypeCombo::TaskTypeCombo(std::shared_ptr<Task> task, QWidget *parent)
    : ComboBox(parent), mTask(task)
{

    //Make sure the global models are populated
    CheckModels();

    //Grab the right model based on the filing type, which is embedded in object:fkObjectType
    //Then set the current key of the combo box to the right value
    switch (task->fkObjectType)
    {
    case ObjectType::filing:
        model = gViewFilingTaskTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case ObjectType::trademarkFiling:
        model = gViewTrademarkFilingTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case ObjectType::patentFiling:
        model = gViewPatentFilingTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case ObjectType::copyrightFiling:
        model = gViewCopyrightFilingTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case ObjectType::response:
        model = gViewResponseTaskTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case ObjectType::patentResponse:
        model = gViewPatentTaskTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case ObjectType::copyrightResponse:
        model = gViewCopyrightTaskTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    default: // General Task
        model = gTaskTypeModel.get();
        model->setSort(2, Qt::SortOrder::AscendingOrder);
        model->select();
        column = 2;
        setToKey(task->fkTaskType);
    }

}

TaskTypeCombo::TaskTypeCombo(const responsesDashboardEntry &entry, QWidget *parent)
    : TaskTypeCombo(std::make_shared<Task>(entry.getTaskId()), parent)
{
    setCurrentText(entry.getTaskName());
}

void TaskTypeCombo::CheckModels()
{
    if(!gViewFilingTaskTypesModel)
        gViewFilingTaskTypesModel = std::make_unique<viewFilingTaskTypesModel>();

    if(!gViewTrademarkFilingTypesModel)
        gViewTrademarkFilingTypesModel = std::make_unique<viewTrademarkFilingTypesModel>();

    if(!gViewPatentFilingTypesModel)
        gViewPatentFilingTypesModel = std::make_unique<viewPatentFilingTypesModel>();

    if(!gViewCopyrightFilingTypesModel)
        gViewCopyrightFilingTypesModel = std::make_unique<viewCopyrightFilingTypesModel>();

    if(!gViewResponseTaskTypesModel)
        gViewResponseTaskTypesModel = std::make_unique<viewResponseTaskTypesModel>();

    if(!gViewPatentTaskTypesModel)
        gViewPatentTaskTypesModel = std::make_unique<viewPatentTaskTypesModel>();

    if(!gViewCopyrightTaskTypesModel)
        gViewCopyrightTaskTypesModel = std::make_unique<viewCopyrightTaskTypesModel>();

    if(!gTaskTypeModel)
        gTaskTypeModel = std::make_unique<taskTypeModel>();
}

} // namespace Nutmeg
