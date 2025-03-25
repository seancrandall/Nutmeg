#ifndef NUTMEG_TABLEVIEW_H
#define NUTMEG_TABLEVIEW_H

#include <QObject>
#include <QSqlRelationalDelegate>
#include <QTableView>
#include <QWidget>

namespace Nutmeg
{

class TableView : public QTableView
{
    Q_OBJECT
  public:
    TableView(QWidget *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_TABLEVIEW_H
