#ifndef NUTMEG_INVENTORSSEARCHBOX_H
#define NUTMEG_INVENTORSSEARCHBOX_H

#include <QObject>
#include <QWidget>

#include "widgets/inventorcompleter.h"
#include "widgets/personsearchbox.h"

namespace Nutmeg
{

class InventorsSearchBox : public Nutmeg::PersonSearchBox
{
    Q_OBJECT
  public:
    explicit InventorsSearchBox(QWidget *parent = nullptr);
    explicit InventorsSearchBox(Key initkey, QWidget *parent = nullptr);

    ReadOnlyProperty(currentText) QString InventorName;

    virtual void slotAddRecord(const QString &input) override;

  private:
    InventorCompleter *completer;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // NUTMEG_INVENTORSSEARCHBOX_H
