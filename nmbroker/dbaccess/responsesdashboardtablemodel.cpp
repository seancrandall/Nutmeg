#include "responsesdashboardtablemodel.h"

namespace Nutmeg
{

ResponsesDashboardTableModel::ResponsesDashboardTableModel(QObject *parent) : Nutmeg::TableModel{parent}
{
    setTable("viewResponsesDashboard");
    setEditStrategy(QSqlTableModel::OnFieldChange);
    setRelation(1, QSqlRelation("matter", "MatterId", "AttorneyDocketNumber"));
    setRelation(2, QSqlRelation("taskType", "TaskTypeId", "TaskName"));
    setRelation(4, QSqlRelation("viewClients", "EntityId", "EntityName"));
    setRelation(5, QSqlRelation("viewParalegals", "PersonId", "EntityName"));
    select();
}

QVariant ResponsesDashboardTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == BOOLEAN_COLUMN)
    {
        bool value = QSqlRelationalTableModel::data(index, Qt::EditRole).toBool();
        return value ? "True" : "False";
    }
    if (role == Qt::CheckStateRole && index.column() == BOOLEAN_COLUMN)
    {
        bool value = QSqlRelationalTableModel::data(index, Qt::EditRole).toBool();
        return value ? Qt::Checked : Qt::Unchecked;
    }
    return QSqlRelationalTableModel::data(index, role);
}

bool ResponsesDashboardTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole && index.column() == BOOLEAN_COLUMN)
    {
        return QSqlRelationalTableModel::setData(index, value.toBool() ? 1 : 0, role);
    }
    if (role == Qt::CheckStateRole && index.column() == BOOLEAN_COLUMN)
    {
        return QSqlRelationalTableModel::setData(index, value == Qt::Checked ? 1 : 0, Qt::EditRole);
    }
    return QSqlRelationalTableModel::setData(index, value, role);
}

} // namespace Nutmeg
