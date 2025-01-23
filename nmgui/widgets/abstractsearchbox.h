#ifndef NUTMEG_ABSTRACTSEARCHBOX_H
#define NUTMEG_ABSTRACTSEARCHBOX_H

#include <QComboBox>
#include <QKeyEvent>
#include <QObject>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QWidget>

#include "nutmeg.h"
#include "widgets/combobox.h"

namespace Nutmeg
{

class AbstractSearchBox : public ComboBox
{
    Q_OBJECT
  public:
    AbstractSearchBox(QWidget *parent = nullptr);

    // Properties
    ReadOnlyProperty(getIsNew) bool isNew;
    Property(getInstantInsert, setInstantInsert) bool instantInsert;

    bool getIsNew(void);

    bool getInstantInsert(void) { return mInstantInsert; }

    void setInstantInsert(bool newval) { mInstantInsert = newval; }

  signals:
    void signalKeySelected(Key selectedKey);

  protected:
    virtual void keyPressEvent(QKeyEvent *event) override; // Override to catch the Return key

    // QSqlTableModel *mod;
    // Key mSelectedKey;

  protected slots:
    virtual void slotAddRecord(const QString &input) = 0;

  private:
    bool mIsNew = false;
    bool mInstantInsert = false;
};

} // namespace Nutmeg

#endif // NUTMEG_ABSTRACTSEARCHBOX_H
