#include "abstractsearchbox.h"

namespace Nutmeg
{

AbstractSearchBox::AbstractSearchBox(QWidget *parent) : ComboBox(parent)
{
    setEditable(true);
    setPlaceholderText("Search...");
}

bool AbstractSearchBox::getIsNew(void)
{
    // Iterate through the model to check if the input matches column
    QString input = currentText();
    for (int row = 0; row < mModel->rowCount(); ++row)
    {
        QSqlRecord record = mModel->record(row);
        // column is the property that holds the modelColumn
        if (record.value(column).toString().compare(input, Qt::CaseInsensitive) == 0)
        {
            key = record.value(0).toUInt();
            return false;
        }
    }
    return true;
}

void AbstractSearchBox::keyPressEvent(QKeyEvent *event)
{
    if (instantInsert)
    {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {
            // Get the current text from the QComboBox
            QString input = currentText();
            // Call the slotAddRecord function with the current input
            if (currentText().isEmpty())
            {
                // Empty, so nothing to add
                return;
            }
            else
            {
                slotAddRecord(input);
                return; // Return early to avoid further processing of the Return key
            }
        }
    }

    // Call the base class implementation for other key events
    QComboBox::keyPressEvent(event);
}

} // namespace Nutmeg
