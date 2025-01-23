#ifndef NUTMEG_PERSONSEARCHBOX_H
#define NUTMEG_PERSONSEARCHBOX_H

#include "abstractsearchbox.h"
#include "widgets/peoplecompleter.h"
#include "windows/insertpersondialog.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class PersonSearchBox : public Nutmeg::AbstractSearchBox
{
    Q_OBJECT
  public:
    explicit PersonSearchBox(QWidget *parent);
    explicit PersonSearchBox(Key initkey, QWidget *parent = nullptr);

    ReadOnlyProperty(getFirstName) QString FirstName;
    ReadOnlyProperty(getLastName) QString LastName;

    QString getFirstName(void)
    {
        mFirstName = Nutmeg::inferName(currentText())[0];
        return mFirstName;
    }

    QString getLastName(void)
    {
        mLastName = Nutmeg::inferName(currentText())[1];
        return mLastName;
    }

  public slots:
    virtual void slotAddRecord(const QString &input) override;

  protected:
    QString mFirstName, mLastName;

    PeopleCompleter *completer;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // NUTMEG_PERSONSEARCHBOX_H
