#include "tasktypecombo.h"

namespace Nutmeg
{

TaskTypeCombo::TaskTypeCombo(Task *task, QWidget *parent) : ComboBox(parent), mTask(task)
{
    // QSqlDatabase db = QSqlDatabase::database();
    // model = new QSqlTableModel(this, db);

    // First, figure out whether this is a filing or response
    // bool isResponse;

    // if( (task->fkObjectType >= 9100) && (task->fkObjectType < 9200) )
    // isResponse = true;

    // switch(task->fkObjectType)
    // {
    // case 9100 ... 9199:
    // }

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

    //  if (task->objectType == "patentResponse") {
    // model = new viewPatentTaskTypesModel(this);
    // column = 2;
    // setToKey(task->fkTaskType);
    //  } else if (task->objectType == "trademarkResponse") {
    // model = new viewTrademarkTaskTypesModel(this);
    // column = 2;
    // setToKey(task->fkTaskType);
    //  } else if (task->objectType == "copyrightResponse") {
    // model = new viewTrademarkTaskTypesModel(this);
    // column = 2;
    // setToKey(task->fkTaskType);
    //  } else {
    // model = new taskTypeModel(this);
    // model->setSort(2, Qt::SortOrder::AscendingOrder);
    // model->select();
    // column = 2;
    // setToKey(task->fkTaskType);
    //  }
}

} // namespace Nutmeg
