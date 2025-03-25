#ifndef RECORD_H
#define RECORD_H

#include <QSqlRecord>
#include <QSqlQuery>
#include "dbaccess/tablemodel.h"
#include "nutmeg.h"
#include "logger.h"

/**
 * @brief Nutmeg::record Gets a single record, by primary key, from the given table model
 * @param primaryKey The primary key of the record to access
 * @param model A pointer to the global model that should contain the record.
 * @return Returns a QSqlRecord if one can be found. If one cannot be found, it returns QSqlRecord()
 *
 * This utility function helps to avoid having to manually manage each global table model every time
 * one is called. We don't always know if a table model already exists, because we don't want to load
 * every table on startup. We only want to load the tables we need as we need them.
 *
 * This method first checks to see wehther the given model is alread loaded (e.g., if the pointer is
 * not null, then we have loaded at least some verison of the table). If the pointer is null, we go ahead
 * and instantiate the model.
 *
 * Once we have ensured that it's not a null pointer, we want to see if the key is in the current key
 * list for the table. This check is necessary because sometimes we set or unset a filter on a table
 * for convenience, and sometimes we add new records that are not in the existing instance of the table.
 * Rather than reload the whole table every time we do something, we just check
 * the next time we try to get a record. If the key is found, we return the record (no need to worry about
 * whether the table is complete).
 *
 * If they key cannot be found in the table, then the filter is unset, and we just re-instantiate the pointer,
 * which will result in the table being reloaded with no filter. Then we again check to make sure that the given
 * primary key is there now.
 *
 * If the primary key exuists, it should be found in the table. But as a final last-ditch effort, we try
 * to read the record directly from the default database connection, in case something screwy is going on with
 * QSqlTableModel.
 *
 * If we succeed in getting the reocrd, we return it. If not, we return a blank QSqlRecord()
 */
namespace Nutmeg {

template <typename Type>
QSqlRecord record(Key primaryKey, std::unique_ptr<Type>& model){
    QSqlRecord rec;

    // Ensure model is instantiated if it's null
    if (!model) {
        model = std::make_unique<Type>();
    }

    // Try to fetch the record from the current model data
    //rec = model->keyRecord[primaryKey];
    rec = model->getRecordByPrimaryKey(primaryKey);

    // If record not found, try reloading the model
    if (rec.isEmpty()) {
        model.reset(new Type()); // Reset model to force reload
        //rec = model->keyRecord[primaryKey];
        rec = model->getRecordByPrimaryKey(primaryKey);
    }

    // If still not found, attempt to query directly from the database
    // if (rec.isEmpty()) {
    //     const QString tableName = model->tableName();
    //     const QString primaryKeyName = model->primaryKeyField;

    //     QSqlDatabase db = QSqlDatabase::database();
    //     if (!db.isValid()) {
    //         Logger::LogMessage(QString("Trying to get record from table %1 with key %2, but no database connection established")
    //                                .arg(tableName)
    //                                .arg(QString::number(primaryKey)));
    //         return rec; // Return empty record if no valid database connection
    //     }

    //     QSqlQuery query(db);
    //     const QString queryString = QString("SELECT * FROM %1 WHERE %2 = :key")
    //                                     .arg(tableName)
    //                                     .arg(primaryKeyName);
    //     query.prepare(queryString);
    //     query.bindValue(":key", primaryKey);

    //     if (!query.exec()) {
    //         qWarning() << "Database query error:" << query.lastError().text();
    //         return rec; // Return empty record on query error
    //     }

    //     if (query.next()) {
    //         // Load record fields into QSqlRecord
    //         rec = query.record();
    //     } else {
    //         qDebug() << "Record not found in database for key:" << primaryKey;
    //     }
    // }

    return rec;
}

} //namespace Nutmeg

#endif // RECORD_HH
