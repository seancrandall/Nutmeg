#include "tableview.h"

namespace Nutmeg
{

TableView::TableView(QWidget *parent) : QTableView(parent) { setItemDelegate(new QSqlRelationalDelegate(parent)); }

} // namespace Nutmeg
