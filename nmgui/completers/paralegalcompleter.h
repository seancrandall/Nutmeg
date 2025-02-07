#ifndef PARALEGALCOMPLETER_H
#define PARALEGALCOMPLETER_H

#include "completer.h"
#include "dbaccess/viewparalegalsmodel.h"

namespace Nutmeg {

class ParalegalCompleter : public Nutmeg::Completer
{
    Q_OBJECT
public:
    explicit ParalegalCompleter(QObject *parent = nullptr);

private:
    viewParalegalsModel *mod;
};

} // namespace Nutmeg

#endif // PARALEGALCOMPLETER_H
