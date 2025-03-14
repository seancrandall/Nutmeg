#include "entitiessearchbox.h"
#include "dbaccess/models.h"
#include "widgets/informationbox.h"
#include "windows/insertentitydialog.h"

namespace Nutmeg
{

EntitiesSearchBox::EntitiesSearchBox(QWidget *parent) : Nutmeg::AbstractSearchBox(parent)
{
    Initialize();
    key = 0;
}

EntitiesSearchBox::EntitiesSearchBox(Key initkey, QWidget *parent) : Nutmeg::AbstractSearchBox(parent)
{
    Initialize();
    key = initkey;
    // emit signalKeySelected(key);
}

void EntitiesSearchBox::slotAddRecord(const QString &input)
{
    Q_UNUSED(input)

    if (isNew)
    {
        QString newname = currentText();
        InsertEntityDialog *diag = new InsertEntityDialog(newname);
        key = diag->exec();
    }
    else
    {
        // Nothing to do. Record already exists
    }
}

void EntitiesSearchBox::Initialize()
{
    if(!gViewEntitiesModel)
        gViewEntitiesModel = std::make_unique<viewEntitiesModel>();
    mModel = gViewEntitiesModel.get();
    setTableModel(mModel);
    column = 1;

    setMinimumWidth(120);

    completer = new EntitiesCompleter(this);
    setCompleter(completer);
}

} // namespace Nutmeg
