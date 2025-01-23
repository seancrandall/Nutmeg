#ifndef NUTMEG_MATTERCOMPLETER_H
#define NUTMEG_MATTERCOMPLETER_H

#include <QObject>

#include "dbaccess/viewmattersmodel.h"
#include "widgets/completer.h"

namespace Nutmeg
{

class MatterCompleter : public Nutmeg::Completer
{
    Q_OBJECT
  public:
    explicit MatterCompleter(QObject *parent = nullptr);

  private:
    viewMattersModel *mod;
};

} // namespace Nutmeg

#endif // NUTMEG_MATTERCOMPLETER_H
