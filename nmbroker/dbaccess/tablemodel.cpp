#include <QSqlIndex>

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
    //if (!mIsLoaded) return QSqlRecord();

    if (!mKeyLocations.contains(primaryKey)) {
        Logger::LogMessage(QString("Tried to get a record for key %1, but it wasn't found in the table.").arg(QString::number(primaryKey)));
        return QSqlRecord();
    }

    QPersistentModelIndex persistentIndex = mKeyLocations[primaryKey];
    if (!persistentIndex.isValid()) {
        Logger::LogMessage(QString("The persistent index for key %1 is no longer valid.").arg(QString::number(primaryKey)));
        return QSqlRecord();
    }

    QModelIndex modelIndex = persistentIndex;
    return record(modelIndex.row());
}

/**
 * @brief TableModel::getRowByPrimaryKey Gets the ordinal row of the table that has the given primary key
 * @param key The primary key to search for
 * @return Returns the integer row that includes the primary key. If no row contains the key, returns a negative value.
 */
int TableModel::getRowByPrimaryKey(Key key) const
{
    if (mKeyLocations.contains(key)) {
        QPersistentModelIndex persistentIndex = mKeyLocations[key];
        if (persistentIndex.isValid()) {
            return persistentIndex.row();
        } else {
            Logger::LogMessage(QString("The persistent index for key %1 is no longer valid.").arg(QString::number(key)));
        }
    } else {
        Logger::LogMessage(QString("Tried to get a row number for primary key %1, but the table does not contain the key.").arg(QString::number(key)));
    }
    return -1;
}

const QString TableModel::getPrimaryKeyField() const
{
    QSqlIndex primaryKeyIndex = this->primaryKey();

    if (!primaryKeyIndex.isEmpty()) {
        // Assuming the primary key is the first (and usually only) field in the index
        return primaryKeyIndex.fieldName(0);
    }

    // If no primary key is defined or found, return an empty string or handle this case appropriately
    return QString();
}

bool TableModel::select()
{
    bool result = QSqlTableModel::select();
    if(result)
        IndexLocations();

    return result;
}

/**
 * @brief TableModel::IndexLocations Populates the mKeyLocations hash so that we have an
 * index of which locations in the table correspond to which primary key values.
 *
 * Child classes should call this from their constructors after loading the actual table into memory
 *
 * Result is that mKeyLocations should be indexable by primary key. For example, if
 * you call mKeyLocations[74] and it returns a valid QPersistentModelIndex,
 * using the row() of that index should give you the record with primary key 74.
 */
void TableModel::IndexLocations()
{
    mKeyLocations.clear();
    // if(!mIsLoaded) {
    //     Logger::LogMessage("Trying to index locations in a TableModel, but the model is not loaded.");
    //     return;
    // }

    for (int row = 0; row < rowCount(); row++) {
        QModelIndex index = this->index(row, 0);
        bool ok;
        Key key = this->data(index).toUInt(&ok);
        if (!ok) {
            Logger::LogMessage("Failed to convert key to unsigned int.");
            continue; // or handle this failure more gracefully
        }

        // Insert the primary key and its corresponding persistent index into the hash
        mKeyLocations.insert(key, QPersistentModelIndex(index));
    }
}

} // namespace Nutmeg
