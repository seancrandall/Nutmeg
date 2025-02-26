#ifndef NUTMEG_INVENTORBUTTON_H
#define NUTMEG_INVENTORBUTTON_H

#include <QHBoxLayout>
#include <QObject>
#include <QWidget>

#include "nutmeg.h"
#include "objects/person.h"
#include "property.h"
#include "widgets/abstractremovablebutton.h"

namespace Nutmeg
{

class InventorButton : public AbstractRemovableButton
{
    Q_OBJECT
  public:
    explicit InventorButton(QWidget *parent = nullptr);
    explicit InventorButton(Key id, QWidget *parent = nullptr);

    virtual void setKey(Key newkey) override;

  signals:
    void signalRemoveInventor(Key inventorId);
    void signalOpenInventor(Key inventorId);

  protected:
    std::shared_ptr<Person> mInventor;

    void setupInventorDisplay(void);
    void ConnectSignalsAndSlots(void);

protected slots:
    void slotReceiveOpenAction(void);
    void slotReceiveRemoveAction(void);
};

} // namespace Nutmeg

#endif // NUTMEG_INVENTORBUTTON_H
