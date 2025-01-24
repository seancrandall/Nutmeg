#ifndef NUTMEG_COMBOBOX_H
#define NUTMEG_COMBOBOX_H

#include <QComboBox>
#include <QEvent>
#include <QObject>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QWidget>

#include "nutmeg.h"
#include "property.h"

namespace Nutmeg
{

class ComboBox : public QComboBox
{
    Q_OBJECT
  public:
    ComboBox(QWidget *parent = nullptr);

    Property(getKey, setToKey) Key key;
    Property(getTableModel, setTableModel) QSqlTableModel *model;
    Property(getColumn, setColumn) int column;

    QSqlTableModel *getTableModel(void) { return mModel; }

    int getColumn(void) { return modelColumn(); }

    bool getIsNew(void);

  public slots:
    void setTableModel(QSqlTableModel *newmod);
    void setToKey(Key key);

    void setColumn(int newcol) { setModelColumn(newcol); }

    void refresh(void);
    Key getKey(void);

  signals:
    void signalKeyChanged(Key newkey);

  protected:
    QSqlTableModel *mModel;
    // QAbstractItemModel *mModel;
    bool eventFilter(QObject *object, QEvent *event) override;

  protected slots:
};

} // namespace Nutmeg

#endif // NUTMEG_COMBOBOX_H
