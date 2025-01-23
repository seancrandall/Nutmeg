#include "tablemodel.h"

namespace Nutmeg
{

TableModel::TableModel(QObject *parent) : QSqlRelationalTableModel{parent, QSqlDatabase::database()}
{
    setEditStrategy(QSqlTableModel::OnFieldChange);
} // end TableModel constructor

bool TableModel::getValid(void)
{
    QSqlDatabase db = QSqlDatabase::database();
    return db.isValid();
}

const QSqlError TableModel::getLastError(void)
{
    QSqlDatabase db = QSqlDatabase::database();
    return db.lastError();
}

// const QSqlRecord TableModel::getRecordByIndex(Key index) const
// {
//     // Loop through all records in the model
//     for (int row = 0; row < QSqlTableModel::rowCount(); ++row) {
//         // Get the record at the current row
//         QSqlRecord newrecord = record(row);

//         // Check if the primary key at position 0 matches the given index
//         if (newrecord.value(0) == QVariant::fromValue(index)) {
//             return newrecord; // Return the record if there's a match
//         }
//     }

//     // If no match was found, return an empty QSqlRecord
//     return QSqlRecord();
// }

} // namespace Nutmeg
