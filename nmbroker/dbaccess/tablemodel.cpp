#include "tablemodel.h"
#include "logger.h"

namespace Nutmeg
{

TableModel::TableModel(QObject *parent)
    : QSqlRelationalTableModel{parent, QSqlDatabase::database()}
{
    setEditStrategy(QSqlTableModel::OnFieldChange);
}

/**
 * @brief TableModel::getLastError Fetches the last error from the database in case there is a problem
 * @return Returns a QSqlError object that represents the last database error returned
 */
const QSqlError TableModel::getLastError(void)
{
    QSqlDatabase db = QSqlDatabase::database();
    Logger::LogMessage(QString("Last database error was %1.").arg(db.lastError().text()));
    return db.lastError();
}

/**
 * @brief TableModel::getRecordByPrimaryKey Gets the record from the table that has the given primary key in the database
 * @param primaryKey The primary key to search for
 * @return A QSqlRecord taken from the row that has the given primary key
 */
QSqlRecord TableModel::getRecordByPrimaryKey(Key primaryKey) const
{
    if(!mIsLoaded) return QSqlRecord();

    //Do we have that key in the table?
    if (!mKeyLocations.contains(primaryKey)) {
        Logger::LogMessage(QString("Tried to get a record for key %1, but it wasn't found in the table.").arg(QString::number(primaryKey)));
        return QSqlRecord();
    }

    int rownum = mKeyLocations[primaryKey];

    // Check if the row number is valid within the current model
    if (rownum < 0 || rownum >= rowCount()) {
        Logger::LogMessage(QString("Found key %1 at row %2, but the row number is invalid.").arg(QString::number(primaryKey).arg(QString::number(rownum))));
        return QSqlRecord();
    }

    return record(rownum);
}

/**
 * @brief TableModel::getRowByPrimaryKey Gets the ordinal row of the table that has the given primary key
 * @param key The primary key to search for
 * @return Returns the integer row that includes the primary key. If no row contains the key, returns a negative value.
 */
int TableModel::getRowByPrimaryKey(Key key) const
{
    if(mKeyLocations.contains(key))
        return mKeyLocations[key];
    else{
        Logger::LogMessage(QString("Tried to get a row number for primary key %1, but the table does not contain the key.").arg(QString::number(key)));
        return -1;
    }
}

/**
 * @brief TableModel::IndexLocations Populates the mKeyLocations hash so that we have an
 * index of which locations in the table correspond to which primary key values.
 *
 * Child classes should call this from their constructors after loading the actual table into memory
 */
void TableModel::IndexLocations()
{
    mKeyLocations.clear();
    if(!mIsLoaded) {
        Logger::LogMessage("Trying to index locations in a TableModel, but the model is not loaded.");
        return;
    }

    for (int row = 0; row < rowCount(); ++row) {
        // For Nutmeg database, first field is always the primary key
        QModelIndex index = this->index(row, 0);
        Key key = this->data(index).toUInt();

        // Insert the primary key and its corresponding row index into the hash
        mKeyLocations.insert(key, static_cast<uint>(row));
    }
}

} // namespace Nutmeg
