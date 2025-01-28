#include "tasktypecombo.h"

namespace Nutmeg
{

TaskTypeCombo::TaskTypeCombo(Task* task, QWidget *parent) : ComboBox(parent), mTask(task)
{

    //Grab the right model based on the filing type, which is embedded in object:fkObjectType
    //Then set the current key of the combo box to the right value
    switch (task->fkObjectType)
    {
    case 2000: // General filing
        model = new viewFilingTaskTypesModel(this);
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 2100: // Trademark filing
        model = new viewTrademarkFilingTypesModel(this);
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 2200: // Patent Filing
        model = new viewPatentFilingTypesModel(this);
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 2300: // Copyright filing
        model = new viewCopyrightFilingTypesModel(this);
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 9100: // General response
        model = new viewResponseTaskTypesModel(this);
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 9120: // Patent Response
        model = new viewPatentTaskTypesModel(this);
        column = 2;
        setToKey(task->fkTaskType);
        break;
    case 9160: // Copyright response
        model = new viewCopyrightTaskTypesModel(this);
        column = 2;
        setToKey(task->fkTaskType);
        break;
    default: // General Task
        model = new taskTypeModel(this);
        model->setSort(2, Qt::SortOrder::AscendingOrder);
        model->select();
        column = 2;
        setToKey(task->fkTaskType);
    }

}

} // namespace Nutmeg
