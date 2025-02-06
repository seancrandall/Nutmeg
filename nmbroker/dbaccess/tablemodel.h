#ifndef NUTMEG_TABLEMODEL_H
#define NUTMEG_TABLEMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QHash>

#include "nutmeg.h"
#include "property.h"

namespace Nutmeg
{

/**
 * @brief The TableModel class is designed for directly viewing or editing
 * tables. The default edit strategy is OnFieldChange, and
 * QSqlRelationalTableModel is exposed as public.
 */
class TableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
  public:
    explicit TableModel(QObject *parent = nullptr);

    ReadOnlyProperty(getLastError) QSqlError lastError; //! The last database error
    ReadOnlyProperty(getRecordByPrimaryKey) QSqlRecord keyRecord[]; //! Gets a QSqlRecord as though the table were an array indexed by primary key
    ReadOnlyProperty(getRowByPrimaryKey) int rowByPrimaryKey[]; //! Returns the ordinal index in the table that corresponds to the given key. For example, if the 7th row has key 482, rowByPrimaryKey[482] will return 6.
    ReadOnlyProperty(getLoaded) bool loaded; //! True if the model is loaded into memory, otherwise false.

    bool getLoaded(void) {return mIsLoaded;}
    const QSqlError getLastError(void);
    QSqlRecord getRecordByPrimaryKey(Key primaryKey) const;
    int getRowByPrimaryKey(Key key) const;

protected:
    void IndexLocations(void);
    QHash<Key, int> mKeyLocations; //! Contains an indexed list of primary keys found in the table.
    bool mIsLoaded = false; //! Child classes should set this to true after loading the model into memory
};

} // namespace Nutmeg

#endif // NUTMEG_TABLEMODEL_H
