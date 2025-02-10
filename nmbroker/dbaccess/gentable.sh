#!/bin/bash

# Check if a parameter was provided
if [ -z "$1" ]; then
    echo "Usage: $0 [TableName]"
    exit 1
fi

TableName=$1
lowerTableName=$(echo "$TableName" | tr '[:upper:]' '[:lower:]')
upperTableName=$(echo "$TableName" | tr '[:lower:]' '[:upper:]')
globalVarName="g$(echo "${TableName:0:1}" | tr '[:lower:]' '[:upper:]')${TableName:1}"

# Create the header file
cat << EOF > "${lowerTableName}model.h"
#ifndef NUTMEG_${upperTableName}MODEL_H
#define NUTMEG_${upperTableName}MODEL_H

#include <QObject>
#include <QSqlRecord>
#include "tablemodel.h"

namespace Nutmeg {

class ${TableName}Model : public Nutmeg::TableModel
{
    Q_OBJECT
public:
    explicit ${TableName}Model(QObject *parent = nullptr);
    
    static QSqlRecord record(Key primaryKey);
};

} // namespace Nutmeg

#endif // NUTMEG_${upperTableName}MODEL_H
EOF

# Create the source file
cat << EOF > "${lowerTableName}model.cpp"
#include "${lowerTableName}model.h"
#include "models.h"
#include "record.h"

namespace Nutmeg {

${TableName}Model::${TableName}Model(QObject *parent)
    : Nutmeg::TableModel{parent}
{
    setTable("${TableName}");
    if(select())
    {
        mIsLoaded = true;
        IndexLocations();
    }
}

QSqlRecord ${TableName}Model::record(Key primaryKey)
{
	//${globalVarName} is already globally defined. 
	return Nutmeg::record<${TableName}Model>(primaryKey, ${globalVarName}Model);
}

} // namespace Nutmeg
EOF

echo "Generated ${lowerTableName}model.h and ${lowerTableName}model.cpp"

