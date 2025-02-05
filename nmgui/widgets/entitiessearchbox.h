#ifndef NUTMEG_ENTITIESSEARCHBOX_H
#define NUTMEG_ENTITIESSEARCHBOX_H

#include "abstractsearchbox.h"
#include "widgets/entitiescompleter.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class EntitiesSearchBox : public Nutmeg::AbstractSearchBox
{
    Q_OBJECT
  public:
    explicit EntitiesSearchBox(QWidget *parent = nullptr);
    explicit EntitiesSearchBox(Key initkey, QWidget *parent = nullptr);

    ReadOnlyProperty(currentText) QString EntityName;

  public slots:
    virtual void slotAddRecord(const QString &input) override;

  private:
    EntitiesCompleter *completer;
    viewEntitiesModel *mModel;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // NUTMEG_ENTITIESSEARCHBOX_H
