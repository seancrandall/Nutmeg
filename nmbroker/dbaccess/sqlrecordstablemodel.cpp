#include "sqlrecordstablemodel.h"

namespace Nutmeg
{

SqlRecordsTableModel::SqlRecordsTableModel(QObject *parent) : QAbstractTableModel(parent) {}

SqlRecordsTableModel::SqlRecordsTableModel(const QList<QSqlRecord> &records, QObject *parent)
    : QAbstractTableModel(parent)
{
    setRecords(records);
}

void SqlRecordsTableModel::setRecords(const QList<QSqlRecord> &records)
{
    beginResetModel();
    mRecords = records;
    endResetModel();
}

int SqlRecordsTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mRecords.size();
}

int SqlRecordsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (mRecords.isEmpty())
    {
        return 0;
    }
    return mRecords.first().count();
}

QVariant SqlRecordsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    const QSqlRecord &record = mRecords.at(index.row());
    return record.value(index.column());
}

QVariant SqlRecordsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal && !mRecords.isEmpty())
    {
        return mRecords.first().fieldName(section);
    }
    else if (orientation == Qt::Vertical)
    {
        return section + 1;
    }

    return QVariant();
}

} // namespace Nutmeg
