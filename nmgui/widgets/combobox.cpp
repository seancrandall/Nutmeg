#include "combobox.h"
#include "logger.h"

namespace Nutmeg
{

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent)
{
    NUTMEGFONT;
    setFocusPolicy(Qt::ClickFocus);

    // Quash the hover scroll
    this->installEventFilter(this);

    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Nutmeg::ComboBox::getKey);
}

void ComboBox::setTableModel(TableModel *newmod)
{
    mModel = newmod;
    QComboBox::setModel(mModel);
    insertItem(0, QString());
}

// void ComboBox::setToKey(QSqlTableModel *model, unsigned int key)
void ComboBox::setToKey(Key key)
{
    // Find the index of the key in the model and set the combo box to it
    for (int row = 0; row < mModel->rowCount(); ++row)
    {
        if (mModel->record(row).value(0).toUInt() == key)
        {
            setCurrentIndex(row);
            break;
        }
    }
}

void ComboBox::refresh(void) { mModel->select(); }

bool ComboBox::eventFilter(QObject *object, QEvent *event)
{
    // Ignore hover events so we don't accidentally scroll combo boxes when the
    // mouse passes over them
    if (event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverMove || event->type() == QEvent::Wheel)
    {
        return true;
    }
    return QComboBox::eventFilter(object, event);
}

Key ComboBox::getKey(void)
{
    int row = currentIndex();
    if (row != -1 && mModel) // Check if we have a valid selection and a model
    {
        QSqlRecord record = mModel->record(row);
        if (!record.isEmpty())
        {
            return record.value(0).toUInt(); // Primary key is in the first column (index 0)
        }
    }
    return 0;
}

} // namespace Nutmeg
