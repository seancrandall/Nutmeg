#include "examinersearchbox.h"
#include "widgets/informationbox.h"
#include "dbaccess/models.h"

namespace Nutmeg
{

ExaminerSearchBox::ExaminerSearchBox(QWidget *parent) : Nutmeg::PersonSearchBox(parent)
{
    Initialize();
    key = 0;
}

ExaminerSearchBox::ExaminerSearchBox(Key initkey, QWidget *parent) : Nutmeg::PersonSearchBox(initkey, parent)
{
    Initialize();
    key = initkey;
    emit signalKeySelected(key);
}

void ExaminerSearchBox::slotAddRecord(const QString &input)
{
    Q_UNUSED(input);
    // if (isNew)
    // {
    //     InsertPersonDialog diag(FirstName, LastName, this);
    //     key = diag.exec();
    //     emit signalKeySelected(key);
    // }
}

void ExaminerSearchBox::Initialize()
{
    if(!gViewPatentExaminersModel)
        gViewPatentExaminersModel = std::make_unique<viewPatentExaminersModel>();
    mModel = gViewPatentExaminersModel.get();

    setTableModel(mModel);
    column = 3;

    setMinimumWidth(120);

    delete PersonSearchBox::completer;
    completer = new ExaminerCompleter(this);
    setCompleter(completer);
}

} // namespace Nutmeg
