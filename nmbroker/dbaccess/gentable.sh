#!/bin/bash

# Check if a parameter was provided
if [ -z "$1" ]; then
    echo "Usage: $0 [TableName]"
    exit 1
fi

TableName=$1
lowerTableName=$(echo "$TableName" | tr '[:upper:]' '[:lower:]')
upperTableName=$(echo "$TableName" | tr '[:lower:]' '[:upper:]')

# Create the header file
cat << EOF > "${lowerTableName}model.h"
#ifndef NUTMEG_${upperTableName}MODEL_H
#define NUTMEG_${upperTableName}MODEL_H

#include <QObject>
#include "tablemodel.h"

namespace Nutmeg {

class ${TableName}Model : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit ${TableName}Model(QObject *parent = nullptr);
};

} // namespace Nutmeg

#endif // NUTMEG_${upperTableName}MODEL_H
EOF

# Create the source file
cat << EOF > "${lowerTableName}model.cpp"
#include "${lowerTableName}model.h"

namespace Nutmeg {

${TableName}Model::${TableName}Model(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("${TableName}");
    select();
}

} // namespace Nutmeg
EOF

echo "Generated ${lowerTableName}model.h and ${lowerTableName}model.cpp"

