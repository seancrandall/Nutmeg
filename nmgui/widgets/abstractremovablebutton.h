#ifndef NUTMEG_ABSTRACTREMOVABLEBUTTON_H
#define NUTMEG_ABSTRACTREMOVABLEBUTTON_H

#include "widgets/informationbox.h"
#include <QHBoxLayout>
#include <QObject>
#include <QWidget>

#include "nutmeg.h"
#include "property.h"

#include "widgets/pushbutton.h"
#include "widgets/xbutton.h"

namespace Nutmeg
{

class AbstractRemovableButton : public QWidget
{
    Q_OBJECT
  public:
    explicit AbstractRemovableButton(QWidget *parent = nullptr);
    // explicit AbstractRemovableButton(Key id, QWidget *parent = nullptr);

    Property(getKey, setKey) Key key;

    Key getKey(void) { return mKey; }

    virtual void setKey(Key newkey) = 0;

  signals:
    void signalRemoveObject(Key removeKey);
    void signalOpenObject(Key openKey);

  public slots:
    virtual void slotRemove(void);
    virtual void slotOpen(void);

  signals:

  protected:
    Key mKey = 0;

    PushButton *cObject = new PushButton();
    XButton *cRemove = new XButton();
    QHBoxLayout *hlayout = new QHBoxLayout();

    void setupDisplay(void);
    void connectSignalsAndSlots(void);
};

} // namespace Nutmeg

#endif // NUTMEG_ABSTRACTREMOVABLEBUTTON_H
