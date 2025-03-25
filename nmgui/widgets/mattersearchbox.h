#ifndef NUTMEG_MATTERSEARCHBOX_H
#define NUTMEG_MATTERSEARCHBOX_H

#include <QLineEdit>
#include <QObject>
#include <QWidget>

#include "widgets/abstractsearchbox.h"
#include "completers/mattercompleter.h"

namespace Nutmeg
{

class MatterSearchBox : public AbstractSearchBox
{
    Q_OBJECT
  public:
    MatterSearchBox(QWidget *parent = nullptr);
    MatterSearchBox(Key initkey, QWidget *parent = nullptr);

    // Properties
    ReadOnlyProperty(currentText) QString AttorneyDocketNumber;

  public slots:
    virtual void slotAddRecord(const QString &input) override;

  private:
    MatterCompleter *completer;
    viewMattersModel *mModel;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // NUTMEG_MATTERSEARCHBOX_H
