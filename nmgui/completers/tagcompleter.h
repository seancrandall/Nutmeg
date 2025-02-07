#ifndef TAGCOMPLETER_H
#define TAGCOMPLETER_H

#include "completer.h"
#include "dbaccess/tagmodel.h"

namespace Nutmeg {

class TagCompleter : public Nutmeg::Completer
{
    Q_OBJECT
public:
    explicit TagCompleter(QObject *parent = nullptr);

private:
    tagModel *mod;
};

} // namespace Nutmeg

#endif // TAGCOMPLETER_H
