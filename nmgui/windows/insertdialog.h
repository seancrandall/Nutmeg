#ifndef NUTMEG_INSERTDIALOG_H
#define NUTMEG_INSERTDIALOG_H

#include "dialog.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class InsertDialog : public Nutmeg::Dialog
{
    Q_OBJECT
  public:
    explicit InsertDialog(QWidget *parent = nullptr);

  signals:
    void signalKeyCreated(Key newkey);

  public slots:
    virtual void slotHandleAccepted(void) override;
    virtual void slotHandleRejected(void) override;

  protected:
    virtual void connectSignalsAndSlots(void) override;

    virtual void slotScatter(void) override {}

    virtual void slotGather(void) override {}

    Property(getNewKey, setNewKey) Key newkey;

    Key getNewKey(void) { return mNewkey; }

    void setNewKey(Key val)
    {
        mNewkey = val;
        setResult(val);
    }

    Key mNewkey = 0;
};

} // namespace Nutmeg

#endif // NUTMEG_INSERTDIALOG_H
