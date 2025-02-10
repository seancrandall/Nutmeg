#ifndef RECORD_H
#define RECORD_H

#include <QSqlRecord>
#include "dbaccess/tablemodel.h"
#include "nutmeg.h"

namespace Nutmeg {

template <typename Type>
QSqlRecord record(Key primaryKey, std::unique_ptr<Type>& model);
}


#endif // RECORD_HH
