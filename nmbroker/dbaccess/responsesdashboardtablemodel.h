#ifndef NUTMEG_RESPONSESDASHBOARDTABLEMODEL_H
#define NUTMEG_RESPONSESDASHBOARDTABLEMODEL_H

#include "tablemodel.h"
#include <QObject>

namespace Nutmeg
{

class ResponsesDashboardTableModel : public Nutmeg::TableModel
{
    Q_OBJECT
  public:
    explicit ResponsesDashboardTableModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

  private:
    static constexpr int BOOLEAN_COLUMN = 13;
};

} // namespace Nutmeg

#endif // NUTMEG_RESPONSESDASHBOARDTABLEMODEL_H
