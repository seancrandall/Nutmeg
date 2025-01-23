#ifndef NUTMEG_TABLEMODEL_H
#define NUTMEG_TABLEMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRelationalTableModel>

#include "nutdb.h"
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

    ReadOnlyProperty(getValid) bool valid;
    ReadOnlyProperty(getLastError) QSqlError lastError;
    //ReadOnlyProperty(getRecordByIndex) QSqlRecord keyRecord[];

    bool getValid(void);                //{return mValid;}
    const QSqlError getLastError(void); // return mNut.db.lastError();}
    //const QSqlRecord getRecordByIndex(Key index) const;

};

} // namespace Nutmeg

#endif // NUTMEG_TABLEMODEL_H
