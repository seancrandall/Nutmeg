#ifndef NUTMEG_PEOPLECOMPLETER_H
#define NUTMEG_PEOPLECOMPLETER_H

#include "completer.h"
#include <QObject>
#include <QWidget>

#include "dbaccess/viewpeoplemodel.h"

namespace Nutmeg
{

class PeopleCompleter : public Nutmeg::Completer
{
    Q_OBJECT
  public:
    explicit PeopleCompleter(QObject *parent = nullptr);

  private:
    viewPeopleModel *mod;

};

} // namespace Nutmeg

#endif // NUTMEG_PEOPLECOMPLETER_H
