#include "tasktypecombo.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

TaskTypeCombo::TaskTypeCombo(std::shared_ptr<Task> task, QWidget *parent)
    : ComboBox(parent), mTask(task)
{

    //Grab the right model based on the filing type, which is embedded in object:fkObjectType
    //Then set the current key of the combo box to the right value
    switch (task->fkObjectType)
    {
    case 2000: // General filing
        model = gViewFilingTaskTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 2100: // Trademark filing
        model = gViewTrademarkFilingTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 2200: // Patent Filing
        model = gViewPatentFilingTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 2300: // Copyright filing
        model = gViewCopyrightFilingTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 9100: // General response
        model = gViewResponseTaskTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 9120: // Patent Response
        model = gViewPatentTaskTypesModel.get();
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 9160: // Copyright response
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
