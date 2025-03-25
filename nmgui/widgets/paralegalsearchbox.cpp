#include "paralegalsearchbox.h"
#include "dbaccess/models.h"

namespace Nutmeg {

ParalegalSearchBox::ParalegalSearchBox(QWidget *parent)
    : PersonSearchBox(parent)
{
    Initialize();
    key = 0;
}

ParalegalSearchBox::ParalegalSearchBox(Key inikey, QWidget *parent)
    : PersonSearchBox(parent)
{
    Initialize();
    key = inikey;
    emit signalKeySelected(inikey);
}

void ParalegalSearchBox::slotAddRecord(const QString &input)
{
    Q_UNUSED(input);

    // if (isNew)
    // {
    //     InsertPersonDialog *diag = new InsertPersonDialog(FirstName, LastName, this);
    //     key = diag->exec();
    // }
}

void ParalegalSearchBox::Initialize()
{
    if(!gViewParalegalsModel)
        gViewParalegalsModel = std::make_unique<viewParalegalsModel>();
    mModel = gViewParalegalsModel.get();
    setTableModel(mModel);
    column = 9;

    setMinimumWidth(120);

    completer = new ParalegalCompleter(this);
    setCompleter(completer);
}

} // namespace Nutmeg


