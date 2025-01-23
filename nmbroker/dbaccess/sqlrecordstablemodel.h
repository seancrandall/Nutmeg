#ifndef NUTMEG_SQLRECORDSTABLEMODEL_H
#define NUTMEG_SQLRECORDSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QSqlRecord>

namespace Nutmeg
{

class SqlRecordsTableModel : public QAbstractTableModel
{
    Q_OBJECT

  public:
    SqlRecordsTableModel(QObject *parent = nullptr);
    SqlRecordsTableModel(const QList<QSqlRecord> &records, QObject *parent = nullptr);

    // Method to set the list of QSqlRecords
    void setRecords(const QList<QSqlRecord> &records);

    // Reimplementing required methods from QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  private:
    QList<QSqlRecord> mRecords;
};

} // namespace Nutmeg

#endif // NUTMEG_SQLRECORDSTABLEMODEL_H
