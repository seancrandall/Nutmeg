#ifndef PARALEGALSEARCHBOX_H
#define PARALEGALSEARCHBOX_H

#include <QObject>
#include <QWidget>

#include "personsearchbox.h"
#include "completers/paralegalcompleter.h"

namespace Nutmeg {

class ParalegalSearchBox : public Nutmeg::PersonSearchBox
{
    Q_OBJECT
public:
    ParalegalSearchBox(QWidget *parent = nullptr);
    ParalegalSearchBox(Key inikey, QWidget *parent = nullptr);

    virtual void slotAddRecord(const QString& input) override;

private:
    ParalegalCompleter *completer;
    viewParalegalsModel *mModel;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // PARALEGALSEARCHBOX_H
