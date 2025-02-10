#include "nutdb.h"
#include "exception.h"
#include<QSqlDriver>

// Key InsertDocumentWithParse(QString FullPath);
namespace Nutmeg
{

uint Nutdb::mErrorCount = 0;
bool Nutdb::mLastOperationSuccessful = 0;
QVector<QString> Nutdb::mErrorList = QVector<QString>();
QList<QPair<Key, QString>> Nutdb::mAllFlags;
QList<QPair<Key, QString>> Nutdb::mAllTags;

Nutdb::Nutdb()
{
}

Nutdb::~Nutdb()
{
}

void Nutdb::AddErrorMessage(const QString msg)
{
    const QString stampedMessage = Logger::LogMessage(msg);
    mErrorList << stampedMessage;
#ifdef QT_DEBUG
    qDebug() << stampedMessage;
#endif
    mLastOperationSuccessful = false;
    ++mErrorCount;
}

///
/// \brief Nutdb::CallStoredProcedure Utility routine to call a stored procedure
/// in the MySQL database
/// \param procName Name of the procedure to call
/// \param parameters QVariantList of the parameters
/// \return true if procedure executed without errors, otherwise false
///
/// WARNING: Prpeared statements don't always work right with this. So this
/// should ONLY be used with stored procedures that are already safe.
QSqlQuery Nutdb::CallStoredProcedure(QString procName, QVariantList parameters, QString outParamName)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    // Escape procName for safety
    procName = db.driver()->escapeIdentifier(procName, QSqlDriver::FieldName);

    QStringList stringParameters;
    for (const QVariant &param : parameters)
    {
        stringParameters << FormatParameter(param);
    }

    // Append the out parameter if provided
    if (!outParamName.isEmpty())
    {
        stringParameters << QString("%1").arg(outParamName);//.replace("'", "''"));
    }

    QString paramlist = stringParameters.join(", ");

    // Construct the CALL statement
    QString sql = QString("CALL %1(%2);")
                      .arg(procName,
                           paramlist);

#ifdef QT_DEBUG
    qDebug() << "Executing query: " << sql;
#endif

    // Execute the query
    bool result = query.exec(sql);
    if (!result)
    {
        qDebug() << "Stored Procedure call failed:" << query.lastError();
#ifdef QT_DEBUG
        QString errmsg = "Attempted call: " + sql + "\n Error message: " + query.lastError().text();
#else
        QString errmsg = "Stored procedure call failed: " + query.lastError().text();
#endif
        Nutdb::AddErrorMessage(errmsg);
        return QSqlQuery(); // Return an invalid query on failure
    }

#ifdef QT_DEBUG
    qDebug() << "Query returned row count of " << query.size();
#endif

    return query; // Return the query object on success
} //end CallStoredProcedure

///
/// \brief Nutdb::FormatParameter Receives a QVariant as an input parameter, and formats it correctly as a string for use in a stored procedure call.
/// \param parameter A QVariant input
/// \return Returns a QString, correctly formatted
///
QString Nutdb::FormatParameter(const QVariant parameter)
{
    if (parameter.isNull())
        return "NULL"; // Return NULL for null values

    QString formattedParam;

    switch (parameter.typeId())
    {
    case QMetaType::QDate:
        // Format QDate as "YYYY-MM-DD"
        formattedParam = parameter.toDate().toString("yyyy-MM-dd");
        break;

    case QMetaType::QDateTime:
        // Format QDateTime as "YYYY-MM-DD HH:MM:SS"
        formattedParam = parameter.toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        break;

    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::Double:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        // Convert numeric types directly to a string
        formattedParam = parameter.toString();
        break;

    case QMetaType::QString:
        // Use the string as-is
        formattedParam = parameter.toString();
        break;

    default:
        // Fallback for other types: attempt to convert to string
        formattedParam = parameter.toString();
        break;
    }

    // Escape single quotes in strings
    formattedParam = formattedParam.replace("'", "''");

    // For non-NULL values, wrap the result in single quotes
    return QString("'%1'").arg(formattedParam);
}

///
/// \brief Nutdb::CallStoredReturnProcedure Utility routine to call a stored
/// procedure in the MySQL database with an output parameter
/// \param procName Name of the procedure to call
/// \param parameters QVariantList of the parameters
/// \return QVariant containing the output parameter value if procedure executed
/// without errors, otherwise a null QVariant
///
QVariant Nutdb::CallStoredReturnProcedure(QString procName, QVariantList parameters)
{
    QString outParamName = "@outId";
    QSqlQuery query;
    // Call the stored procedure with the output parameter
    query = CallStoredProcedure(procName, parameters, outParamName);
    if(!mLastOperationSuccessful)
    {
        return QVariant();
    }

    QString selectOutParam = QString("SELECT %1;").arg(outParamName);
    mLastOperationSuccessful = query.exec(selectOutParam);
    if (!mLastOperationSuccessful)
    {
        qDebug() << "Failed to retrieve OUT parameter value:" << query.lastError();
        QString msg = "Failed to retrieve output procedure for call to " + procName +
                      "().\nLast error: " + query.lastError().text();
        Nutdb::AddErrorMessage(msg);
        return QVariant();
    }

    mLastOperationSuccessful = query.next();
    if (mLastOperationSuccessful)
    {
        return query.value(0);
    }
    else
    {
        QString msg = QString("Something went wrong. The SELECT operation seems to have succeeded, but failed to retrieve an output value from %1.\nLast error was: %2")
        .arg(procName)
            .arg(query.lastError().text());
        Nutdb::AddErrorMessage(msg);
        return QVariant();
    }
}

Key Nutdb::CallStoredKeyProcedure(QString procName, QVariantList parameters)
{
    QVariant result = CallStoredReturnProcedure(procName, parameters);

    //if(mLastOperationSuccessful)
    return result.toUInt();
    //else
    //return 0;
}

bool Nutdb::UpdateField(QString table, QString field, Key key, QString value)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        qDebug() << "Database is not open!";
        Nutdb::AddErrorMessage("Database connection is closed.");
        return false;
    }

    QString primaryKeyName = GetPrimaryKeyName(table);
    QSqlQuery query;

    // Construct the SQL UPDATE query with escaped identifiers
    QString sql = QString("UPDATE %1 SET %2 = :value WHERE %3 = :key")
                      .arg(table,
                           field,
                           primaryKeyName);

    // Prepare the SQL query
    //mLastOperationSuccessful = query.prepare(sql);
    bool result = query.prepare(sql);
    if (!result)
    {
        qDebug() << "Failed to prepare query:" << query.lastError().text();
        QString msg = QString("Failed preparing a query to update field %1 in table %2 with key %3.\nLast error was: %4")
                          .arg(field,
                               table,
                               QVariant::fromValue(key).toString(),
                               query.lastError().text());
        Nutdb::AddErrorMessage(msg);
        return false;
    }

    // Bind the parameters to the query
    query.bindValue(":value", value);
    query.bindValue(":key", static_cast<quint64>(key));

    // Execute the query
    result = query.exec();
    if (!result)
    {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        QString msg = QString("Failed to execute prepared query '%1'. \nLast error was: %2.")
                          .arg(sql,
                               query.lastError().text());
        Nutdb::AddErrorMessage(msg);
        return false;
    }

    return true;
}

///
/// \brief Nutdb::GetRecord Get a single database record
/// \param table The name of the table to select the record from
/// \param id The primary key of the record to select
/// \return Returns a QSqlRecord with the selected record
///
QSqlRecord Nutdb::GetRecord(QString table, Key id)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query = QSqlQuery(db);
    QSqlRecord record;
    QString primaryKeyName;

    primaryKeyName = GetPrimaryKeyName(table);

    // Build the SQL query with table and primary key name directly
    QString sqlQuery = QString("SELECT * FROM %1 WHERE %2 = :id").arg(table).arg(primaryKeyName);

    // Prepare and bind values
    query.prepare(sqlQuery);
    query.bindValue(":id", NullableInteger(id));

    // Execute the query
    if (query.exec() && query.next())
    {
        record = query.record();
    }
    else
    {
        qDebug() << "Error executing query:" << query.lastError().text();
        QString msg = QString("Failed reading from table %1 with key %2.\nReturned error: %3.")
                          .arg(table)
                          .arg(QVariant::fromValue(id).toString())
                          .arg(query.lastError().text());
        Nutdb::AddErrorMessage(msg);
    }

    return record;
}

const QList<QSqlRecord> Nutdb::SearchLike(QString table, QString fieldName, QString searchQString)
{
    QList<QSqlRecord> resultList;

    // Check if the table and field names are valid
    if (table.isEmpty() || fieldName.isEmpty())
    {
        QString msg = QString("Tried to 'Search Like' for table %1 and field %2, but one of them was empty. Weird.")
        .arg(table)
            .arg(fieldName);
        Nutdb::AddErrorMessage(msg);
        return resultList; // Return an empty list if invalid
    }

    // Construct the SQL query
    QString sql = QString("SELECT * FROM %1 WHERE %2 LIKE :searchQString").arg(table, fieldName);

    QSqlQuery query(QSqlDatabase::database());

    mLastOperationSuccessful = query.prepare(sql);
    if(!mLastOperationSuccessful)
    {
        QString errmsg = QString("Tried to prepare the query '%1', but prepare failed.")
        .arg(sql);
        Nutdb::AddErrorMessage(errmsg);
    }
    // Set the search parameter with wildcard
    query.bindValue(":searchQString", QString("%%1%").arg(searchQString));

    // Execute the query
    mLastOperationSuccessful = query.exec();
    if (!mLastOperationSuccessful)
    {
        QString errmsg = QString("Tried to execute query '%1', but execution failed.\nLast Error: %2")
        .arg(sql)
            .arg(query.lastError().text());
        Nutdb::AddErrorMessage(errmsg);
        return resultList; // Return an empty list on error
    }

    // Add all returned records to the resultList
    while (query.next())
    {
        resultList.append(query.record());
    }

    return resultList; // Return the list of records
}

Key Nutdb::AddRoleToEntity(Key entityId, Key roleId)
{
    QVariantList params;

    params.append(QVariant::fromValue(entityId));
    params.append(QVariant::fromValue(roleId));

    return CallStoredKeyProcedure("AddRoleToEntity", params);
}

Key Nutdb::AddRoleToPerson(Key personId, Key roleId)
{
    QVariantList params;

    params.append(QVariant::fromValue(personId));
    params.append(QVariant::fromValue(roleId));

    return CallStoredKeyProcedure("AddRoleToPerson", params);
}

Key Nutdb::InsertClientNatural(QString firstName, QString lastName)
{
    QVariantList params;

    params.append(firstName);
    params.append(lastName);

    return CallStoredKeyProcedure("InsertClientNatural", params);
}

Key Nutdb::InsertCopyrightFiling(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertCopyrightFiling", params);
}

Key Nutdb::InsertCopyrightMatter(QString docketNumber)
{
    QVariantList params;

    params.append(docketNumber);

    return CallStoredKeyProcedure("InsertCopyrightMatter", params);
}

Key Nutdb::InsertDocument(QString title)
{
    QVariantList params;

    params.append(title);

    return CallStoredKeyProcedure("InsertDocument", params);
}

Key Nutdb::InsertEnterprise(QString enterpriseName)
{
    QVariantList params;

    params.append(enterpriseName);

    return CallStoredKeyProcedure("InsertEnterprise", params);
}

Key Nutdb::InsertEntity(QString entityName)
{
    QVariantList params;

    params.append(entityName);

    return CallStoredKeyProcedure("InsertEntity", params);
}

Key Nutdb::InsertExaminer(QString firstName, QString lastName)
{
    QVariantList params;

    params.append(firstName);
    params.append(lastName);

    return CallStoredKeyProcedure("InsertExaminer", params);
}

Key Nutdb::InsertFiling(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertFiling", params);
}

Key Nutdb::InsertFinalOA(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertFinalOA", params);
}

Key Nutdb::InsertGeneralMatter(QString docketNumber)
{
    QVariantList params;

    params.append(docketNumber);

    return CallStoredKeyProcedure("InsertGeneralMatter", params);
}

Key Nutdb::InsertInventor(QString firstName, QString lastName)
{
    QVariantList params;

    params.append(firstName);
    params.append(lastName);

    return CallStoredKeyProcedure("InsertInventor", params);
}

Key Nutdb::InsertMatter(QString docketNumber)
{
    QVariantList params;

    params.append(docketNumber);

    return CallStoredKeyProcedure("InsertMatter", params);
}

Key Nutdb::InsertNonfinalOA(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertNonfinalOA", params);
}

Key Nutdb::InsertNote(Key objectId, QString noteText)
{
    QVariantList params;

    params.append(QVariant::fromValue(objectId));
    params.append(noteText);

    return CallStoredKeyProcedure("InsertNote", params);
}

Key Nutdb::InsertObject(QString objectType)
{
    QVariantList params;

    params.append(objectType);

    return CallStoredKeyProcedure("InsertObject", params);
}

Key Nutdb::InsertParalegal(QString firstName, QString lastName)
{
    QVariantList params;

    params.append(firstName);
    params.append(lastName);

    return CallStoredKeyProcedure("InsertParalegal", params);
}

Key Nutdb::InsertPatentExaminer(QString firstName, QString lastName)
{
    QVariantList params;

    params.append(firstName);
    params.append(lastName);

    return CallStoredKeyProcedure("InsertPatentExaminer", params);
}

Key Nutdb::InsertPatentFiling(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertPatentFiling", params);
}

Key Nutdb::InsertPatentMatter(QString docketNumber)
{
    QVariantList params;

    params.append(docketNumber);

    return CallStoredKeyProcedure("InsertPatentMatter", params);
}

Key Nutdb::InsertPatentResponse(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertPatentResponse", params);
}

Key Nutdb::InsertPerson(QString firstName, QString lastName)
{
    QVariantList params;

    params.append(firstName);
    params.append(lastName);

    Key result = CallStoredKeyProcedure("InsertPerson", params);
    if(result){
        gViewPeopleModel = std::make_unique<viewPeopleModel>();
        gViewInventorsModel = std::make_unique<viewInventorsModel>();
        gViewPatentExaminersModel = std::make_unique<viewPatentExaminersModel>();
    }
    return result;
}

Nutmeg::Key Nutdb::InsertResponse(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertResponse", params);
}

Key Nutdb::InsertTask(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTask", params);
}

Key Nutdb::InsertTaskNinety(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTaskNinety", params);
}

Key Nutdb::InsertTaskSixty(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(NullableInteger(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTaskSixty", params);
}

Key Nutdb::InsertTaskThirty(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTaskThirty", params);
}

Key Nutdb::InsertTaskThreeMonth(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTaskThreeMonth", params);
}

Key Nutdb::InsertTaskThreeMonthHard(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(NullableInteger(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTaskThreeMonthHard", params);
}

Key Nutdb::InsertTaskTwoMonth(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTaskTwoMonth", params);
}

Key Nutdb::InsertTaskWithMatter(QString docketNumber, Date triggerDate)
{
    QVariantList params;

    params.append(docketNumber);
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTaskWithMatter", params);
}

Key Nutdb::InsertTrademarkFiling(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTrademarkFiling", params);
}

Key Nutdb::InsertTrademarkMatter(QString docketNumber)
{
    QVariantList params;

    params.append(docketNumber);

    return CallStoredKeyProcedure("InsertTrademarkMatter", params);
}

Key Nutdb::InsertTrademarkResponse(Key matterId, Date triggerDate)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(triggerDate);

    return CallStoredKeyProcedure("InsertTrademarkResponse", params);
}

Key Nutdb::InsertWorkAttorney(QString firstName, QString lastName)
{
    QVariantList params;

    params.append(firstName);
    params.append(lastName);

    return CallStoredKeyProcedure("InsertWorkAttorney", params);
}

Key Nutdb::MergeTags(Key tagOneId, Key tagTwoId, QString tagText)
{
    QVariantList params;

    params.append(QVariant::fromValue(tagOneId));
    params.append(QVariant::fromValue(tagTwoId));
    params.append(tagText);

    return CallStoredKeyProcedure("MergeTags", params);
}

Key Nutdb::SetFlag(Key associatedObjectId, QString camelCase)
{
    QVariantList params;

    params.append(QVariant::fromValue(associatedObjectId));
    params.append(camelCase);

    return CallStoredKeyProcedure("SetFlag", params);
}

Key Nutdb::TagObject(Key objectId, QString tagText)
{
    QVariantList params;

    params.append(QVariant::fromValue(objectId));
    params.append(tagText);

    return CallStoredKeyProcedure("TagObject", params);
}

ActionData Nutdb::GetAction(Key input)
{
    ActionData dat;
    QSqlRecord rec = GetRecord("action", input);

    dat.ActionId = rec.field("ActionId").value().toUInt();
    dat.ActionName = rec.field("ActionName").value().toString();

    return dat;
}

AppointmentData Nutdb::GetAppointment(Key input)
{
    AppointmentData dat;
    QSqlRecord rec = GetRecord("appointment", input);

    dat.AppointmentId = rec.field("AppointmentId").value().toUInt();
    dat.AppointmentTime = rec.field("AppointmentTime").value().toDateTime();
    dat.fkAppointmentType = rec.field("fkAppointmentType").value().toUInt();
    return dat;
}

QList<Key> Nutdb::GetCaseInventors(Key id)
{
    QList<Key> resultList;
    QSqlDatabase db = QSqlDatabase::database(); // Use the default connection
    QSqlQuery query(db);

    // Prepare the SQL query
    query.prepare("SELECT fkPerson FROM patentMatter_inventor WHERE fkPatentMatter = :id");
    query.bindValue(":id", QVariant::fromValue(id));

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Failed to execute query:" << query.lastError().text();
        return resultList; // Return empty list on failure
    }

    // Fetch results
    while (query.next()) {
        // Append each fkPerson value to the result list
        resultList.append(query.value(0).toUInt());
    }

    return resultList;
}

QList<Key> Nutdb::GetObjectAppointments(Key id)
{
    QList<Key> resultList;

    QVariantList params;
    params.append(NullableInteger(id));

    QSqlQuery query;
    query = CallStoredProcedure("GetObjectAppointments", params);
    if(!mLastOperationSuccessful)
        return resultList;

    while (query.next())
        resultList.append(query.record().field(0).value().toUInt());

    return resultList;
}

CopyrightFilingData Nutdb::GetCopyrightFiling(Key id)
{
    CopyrightFilingData dat = CopyrightFilingData();
    QSqlRecord rec;
    rec = GetRecord("copyrightFiling", id);
    if(!mLastOperationSuccessful)
        return dat;

    dat.CopyrightFilingId = rec.field(0).value().toUInt();
    return dat;
}

CopyrightFormData Nutdb::GetCopyrightForm(Key id)
{
    CopyrightFormData dat;
    QSqlRecord rec;
    rec = GetRecord("copyrightForm", id);
    if(!mLastOperationSuccessful)
        return dat;

    dat.CopyrightFormId = rec.field(0).value().toUInt();
    dat.FormName = rec.field("FormName").value().toString();
    dat.fkStoredForm = rec.field("fkStoredForm").value().toUInt();

    return dat;
}

CopyrightMatterData Nutdb::GetCopyrightMatter(Key id)
{
    CopyrightMatterData dat;
    QSqlRecord rec;
    rec = GetRecord("copyrightMatter", id);
    if(!mLastOperationSuccessful)
        return dat;

    dat.CopyrightMatterId = rec.field(0).value().toUInt();
    dat.fkAuthor = rec.field("fkAuthor").value().toUInt();
    dat.Created = rec.field("Created").value().toDate();
    dat.Filed = rec.field("Filed").value().toDate();
    dat.Registered = rec.field("Registered").value().toDate();
    dat.SerialNumber = rec.field("SerialNumber").value().toString();
    dat.RegistrationNumber = rec.field("RegistrationNumber").value().toString();
    dat.fkDeposit = rec.field("fkDeposit").value().toUInt();
    dat.fkWorkType = rec.field("fkWorkType").value().toUInt();

    return dat;
}

DeadlineData Nutmeg::Nutdb::GetDeadline(Key id)
{
    DeadlineData dat;
    QSqlRecord rec;

    rec = deadlineModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.DeadlineId = rec.KeyField("DeadlineId");
    dat.TriggerDate = rec.DateField("TriggerDate");
    dat.SoftDeadline = rec.DateField("SoftDeadline");
    dat.HardDeadline = rec.DateField("HardDeadline");
    dat.NextDeadline = rec.DateField("NextDeadline");

    return dat;
}

DocumentData Nutdb::GetDocument(Key id)
{
    DocumentData dat;
    QSqlRecord rec;
    rec = GetRecord("document", id);
    if(!mLastOperationSuccessful)
        return dat;

    dat.DocumentId = rec.UIntField("DocumentId");
    dat.Extension = rec.StringField("Extension");
    dat.Filename = rec.StringField("Filename");
    dat.FullyQualifiedPath = rec.StringField("FullyQualifiedPath");
    dat.Title = rec.StringField("Title");
    dat.URL = rec.StringField("URL");

    return dat;
}

QList<Key> Nutdb::GetDocumentObjects(Key documentId)
{
    QVariantList params;
    QList<Key> objectKeyList;
    params.append(NullableInteger(documentId));

    QSqlQuery query;
    query = CallStoredProcedure("GetDocumentObjects", params);
    if(!mLastOperationSuccessful)
        return objectKeyList;

    while (query.next())
        objectKeyList << query.record().field(0).value().toUInt();

    return objectKeyList;
}

QList<Key> Nutdb::GetObjectDocuments(Key objectId)
{
    QSqlQuery query;
    QVariantList params;
    QList<Key> documentList;

    params.append(NullableInteger(objectId));

    query = CallStoredProcedure("GetObjectDocuments", params);
    if(!mLastOperationSuccessful)
        return documentList;

    while (query.next())
        documentList << query.record().field(0).value().toUInt();

    return documentList;
}

EnterpriseData Nutdb::GetEnterprise(Key id)
{
    QSqlRecord rec;
    EnterpriseData dat;

    rec = viewEntitiesModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.EnterpriseId = rec.KeyField("EnterpriseId");
    dat.EnterpriseName = rec.StringField("EnterpriseName");
    dat.fkBusinessJurisdiction = rec.KeyField("fkBusinessJurisdiction");
    dat.fkStateOfIncorporation = rec.KeyField("fkStateOfIncorporation");
    dat.fkMainContact = rec.KeyField("fkMainContact");
    dat.OldOrganizationId = rec.KeyField("OldOrganizationId");

    return dat;
}

Key Nutdb::InsertTag(QString tagText)
{
    QVariantList params;

    params.append(tagText);

    return CallStoredKeyProcedure("InsertTag", params);
}

EntityData Nutdb::GetEntity(Key id)
{
    QSqlRecord rec;
    EntityData dat;

    rec = viewEntitiesModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.EntityId = rec.UIntField("EntityId");
    dat.EntityName = rec.StringField("EntityName");
    dat.FullLegalName = rec.StringField("FullLegalName");
    dat.DocketPrefix = rec.StringField("DocketPrefix");
    dat.PrimaryPhone = rec.StringField("PrimaryPhone");
    dat.SecondaryPhone = rec.StringField("SecondaryPhone");
    dat.FaxNumber = rec.StringField("FaxNumber");
    dat.PrimaryAddress = rec.StringField("PrimaryAddress");
    dat.SecondaryAddress = rec.StringField("SecondaryAddress");
    dat.PrimaryEmail = rec.StringField("PrimaryEmail");
    dat.SecondaryEmail = rec.StringField("SecondaryEmail");
    dat.Website = rec.StringField("Website");
    dat.fkJurisdiction = rec.UIntField("fkJurisdiction");
    dat.fkState = rec.UIntField("fkState");

    return dat;
}

FilingData Nutdb::GetFiling(Key id)
{
    FilingData dat;
    QSqlRecord rec;

    rec = viewFilingsModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.FilingId = rec.KeyField("FilingId");
    dat.fkFilingStatus = rec.KeyField("fkFilingStatus");
    dat.fkAsFiledDocument = rec.KeyField("fkAsFiledDocument");

    return dat;
}

bool Nutdb::GetFlag(Key objectId, QString camelCase)
{
    if(!gViewObjectFlagsModel)
        gViewObjectFlagsModel = std::make_unique<viewObjectFlagsModel>();

    bool flagValue = false
        , found = false;
    QSqlRecord rec;

    gViewObjectFlagsModel->setFilter(QString("fkObject = %1").arg(objectId));

    //++i
    for(auto i=0; i < gViewObjectFlagsModel->rowCount(); i++){
        QString camel;
        camel = gViewObjectFlagsModel->record(i).field("CamelCase").value().toString();
        if(camel == camelCase){
            found = true;
            flagValue = gViewObjectFlagsModel->record(i).field("FlagValue").value().toBool();
            break;
        }
    }

    //If we couldn't find it in the loaded table, try a native database call
    //as a backup
    if(!found){
        QVariantList params;

        params.append(NullableInteger(objectId));
        params.append(camelCase);

        QVariant flagval;
        QVariant result = CallStoredReturnProcedure("GetFlag", params);
        if(!mLastOperationSuccessful)
            return false;
        else
            flagValue = result.toBool();
    }
    return flagValue;
}

FlagClassData Nutdb::GetFlagClass(QString camelCase)
{
    FlagClassData dat;
    QSqlRecord rec;

    if(!gFlagClassModel)
        gFlagClassModel = std::make_unique<flagClassModel>();

    Key flagKey = gFlagClassModel->camelCase[camelCase];
    rec = gFlagClassModel->keyRecord[flagKey];

    if(rec.isEmpty())
        return dat;

    dat.FlagClassId = rec.field("FlagClassId").value().toUInt();
    dat.CamelCase = rec.field("CamelCase").value().toString();
    dat.Label = rec.field("Label").value().toString();
    dat.Description = rec.field("Description").value().toString();

    return dat;
}

FlagClassData Nutdb::GetFlagClass(Key id)
{
    FlagClassData dat;
    QSqlRecord rec;

    rec = flagClassModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.FlagClassId = rec.field("FlagClassId").value().toUInt();
    dat.CamelCase = rec.field("CamelCase").value().toString();
    dat.Label = rec.field("Label").value().toString();
    dat.Description = rec.field("Description").value().toString();

    return dat;

}

MatterData Nutdb::GetMatter(Key id)
{
    QSqlRecord rec;
    MatterData dat;

    rec = viewMattersModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.MatterId = rec.KeyField("MatterId");
    dat.fkParent = rec.KeyField("fkParent");
    dat.AttorneyDocketNumber = rec.StringField("AttorneyDocketNumber");
    dat.ClientDocketNumber = rec.StringField("ClientDocketNumber");
    dat.Title = rec.StringField("Title");
    dat.fkClient = rec.KeyField("fkClient");
    dat.fkAssigningFirm = rec.KeyField("fkAssigningFirm");
    dat.fkDefaultWorkAttorney = rec.KeyField("fkDefaultWorkAttorney");
    dat.fkDefaultParalegal = rec.KeyField("fkDefaultParalegal");
    dat.fkKeyDocument = rec.KeyField("fkKeyDocument");
    dat.fkMatterJurisdiction = rec.KeyField("fkMatterJurisdiction");
    dat.OldMatterId = rec.KeyField("OldMatterId");

    return dat;
}

ObjectData Nutdb::GetObject(Key id)
{
    QSqlRecord rec;
    ObjectData dat;

    rec = objectModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.ObjectId = rec.KeyField("ObjectId");
    dat.fkObjectType = rec.KeyField("fkObjectType");

    return dat;
}

QList<FlagData> Nutdb::GetObjectFlags(Key objectId)
{
    QList<FlagData> flagList;

    if(!gViewObjectFlagsModel)
        gViewObjectFlagsModel = std::make_unique<viewObjectFlagsModel>();

    //Filter for the given objectId
    gViewObjectFlagsModel->setFilter(QString("ObjectId = %1").arg(objectId));
    //++i
    for(int i=0; i < gViewObjectFlagsModel->rowCount(); i++)
    {
        FlagData localdata;
        QSqlRecord rec = gViewObjectFlagsModel->record(i);
        localdata.CamelCase = rec.field("CamelCase").value().toString();
        localdata.Description = rec.field("Description").value().toString();
        localdata.Label = rec.field("Label").value().toString();
        localdata.FlagClassId = rec.field("fkFlag").value().toUInt();
        localdata.FlagValue = rec.field("FlagValue").value().toBool();
        flagList << localdata;
    }

    //Now clear the filter
    gViewObjectFlagsModel->setFilter(QString());

    return flagList;
}

QList<Key> Nutdb::GetFlagObjects(const QString &camelCase)
{
    QList<Key> list;

    if(!gViewObjectFlagsModel)
        gViewObjectFlagsModel = std::make_unique<viewObjectFlagsModel>();

    gViewObjectFlagsModel->setFilter(QString("CamelCase = %1").arg(camelCase));

    for(int i=0; i < gViewObjectFlagsModel->rowCount(); i++){
        list << gViewObjectFlagsModel->record(i).field("ObjectId").value().toUInt();
    }

    gViewObjectFlagsModel->setFilter(QString());

    return list;
}

QString Nutdb::GetObjectTypeString(Key objectTypeId)
{
    ObjectTypeData dat;
    QVariantList params;

    params.append(NullableInteger(objectTypeId));

    return CallStoredReturnProcedure("GetObjectTypeString", params).toString();
}

Key Nutdb::GetObjectTypeId(QString objectTypeText)
{
    ObjectTypeData dat;

    QVariantList params;
    params.append(objectTypeText);

    return CallStoredReturnProcedure("GetObjectTypeId", params).toUInt();
}

QList<TagData> Nutdb::GetObjectTags(Key objectId)
{
    QList<TagData> tagList;

    if(!gViewObjectTagsModel)
        gViewObjectTagsModel = std::make_unique<viewObjectTagsModel>();

    //Filter for the given objectId
    gViewObjectTagsModel->setFilter(QString("ObjectId = %1").arg(objectId));

    for(int i = 0; i < gViewObjectTagsModel->rowCount(); i++){
        TagData localdat;
        QSqlRecord rec = gViewObjectTagsModel->record(i);
        localdat.TagId = rec.field("TagId").value().toUInt();
        localdat.TagText = rec.field("TagText").value().toString();
        tagList << localdat;
    }

    //Clear the filter
    gViewObjectTagsModel->setFilter(QString());
    return tagList;
}

QList<Key> Nutdb::GetTagObjects(const QString &tagText)
{
    QSqlQuery query;
    QList<Key> resultList;

    QVariantList params;
    params.append(tagText);

    query = CallStoredProcedure("GetTagObjects", params);
    if(!mLastOperationSuccessful)
        return resultList;

    while (query.next())
        resultList.append(query.record().field(0).value().toUInt());

    return resultList;
}

PatentMatterData Nutdb::GetPatentMatter(Key id)
{
    QSqlRecord rec;
    PatentMatterData dat;

    rec = viewPatentMattersModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.PatentMatterId = rec.KeyField("PatentMatterId");
    dat.FilingDate = rec.field("FilingDate").value().toDate();
    dat.ApplicationSerialNumber = rec.StringField("ApplicationSerialNumber");
    dat.ConfirmationNumber = rec.StringField("ConfirmationNumber");
    dat.ArtUnit = rec.StringField("ArtUnit");
    dat.PatentNumber = rec.StringField("PatentNumber");
    dat.fkExaminer = rec.KeyField("fkExaminer");
    dat.fkFirstInventor = rec.KeyField("fkFirstInventor");
    dat.fkSupervisoryExaminer = rec.KeyField("fkSupervisoryExaminer");
    dat.fkApplicant = rec.KeyField("fkApplicant");
    dat.BarDate = rec.field("BarDate").value().toDate();
    dat.CriticalDate = rec.field("CriticalDate").value().toDate();
    dat.fkSpecification = rec.KeyField("fkSpecification");
    dat.fkDrawings = rec.KeyField("fkDrawings");
    dat.fkAsFiledClaims = rec.KeyField("fkAsFiledClaims");

    return dat;
}

PersonData Nutdb::GetPerson(Key id)
{
    QSqlRecord rec;
    PersonData dat;

    rec = viewPeopleModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.PersonId = rec.UIntField("PersonId");
    dat.FirstName = rec.StringField("FirstName");
    dat.LastName = rec.StringField("LastName");
    dat.OldId = rec.UIntField("OldId");
    dat.fkCitizenship = rec.UIntField("fkCitizenship");
    dat.fkResidence = rec.UIntField("fkResidence");

    return dat;
}

ResponseData Nutdb::GetResponse(Key id)
{
    QSqlRecord rec;
    ResponseData dat;

    rec = viewResponsesModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.ResponseId = rec.KeyField("ResponseId");
    dat.fkClientOfficeHours = rec.KeyField("fkClientOfficeHours");
    dat.fkExaminerInterview = rec.KeyField("fkExaminerInterview");
    dat.MailingDate = rec.DateField("MailingDate");
    dat.DateFiled = rec.DateField("DateFiled");
    dat.fkResponseAsFiled = rec.KeyField("fkResponseAsFiled");
    dat.fkActionDocument = rec.KeyField("fkActionDocument");

    return dat;
}

TagData Nutdb::GetTag(Key id)
{
    TagData dat;
    QSqlRecord rec;

    rec = tagModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.TagId = rec.UIntField("TagId");
    dat.TagText = rec.StringField("TagText");

    return dat;
}

Key Nutdb::GetTagId(QString tagText)
{
    if(!gTagModel)
        gTagModel = std::make_unique<tagModel>();

    Key tagId = gTagModel->tagId[tagText];
    return tagId;
}

TaskData Nutdb::GetTask(Key id)
{
    TaskData dat;
    QSqlRecord rec;

    rec = viewTasksModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.TaskId = rec.KeyField("TaskId");
    dat.fkMatter = rec.KeyField("fkMatter");
    dat.DateAssigned = rec.DateField("DateAssigned");
    dat.fkDeadline = rec.KeyField("fkDeadline");
    dat.ExpectedFee = rec.FloatField("ExpectedFee");
    dat.FeePercent = rec.FloatField("FeePercent");
    dat.ApprovalRequested = rec.DateField("ApprovalRequested");
    dat.ApprovalReceived = rec.DateField("ApprovalReceived");
    dat.Complete = rec.BoolField("Complete");
    dat.fkWorkAttorney = rec.KeyField("fkWorkAttorney");
    dat.fkParalegal = rec.KeyField("fkParalegal");
    dat.fkAuthorizationDocument = rec.KeyField("fkAuthorizationDocument");
    dat.OldTaskId = rec.KeyField("OldTaskId");
    dat.fkTaskType = rec.KeyField("fkTaskType");

    return dat;
}

TrademarkMatterData Nutdb::GetTrademarkMatter(Key id)
{
    TrademarkMatterData dat;
    QSqlRecord rec;

    rec = viewTrademarkMattersModel::record(id);

    if(rec.isEmpty())
        return dat;

    dat.TrademarkMatterId = rec.KeyField("TrademarkMatterId");
    dat.FirstUseInCommerce = rec.DateField("FirstUseInCommerce");
    dat.InternationalClass = rec.IntField("InternationalClass");
    dat.fkStatus = rec.KeyField("fkStatus");
    dat.SerialNumber = rec.StringField("SerialNumber");
    dat.RegistrationNumber = rec.StringField("RegistrationNumber");
    dat.PublicationDate = rec.DateField("PublicationDate");
    dat.WindowOpens = rec.DateField("WindowOpens");
    dat.NofeeWindowCloses = rec.DateField("NofeeWindowCloses");
    dat.FinalWindowCloses = rec.DateField("FinalWindowCloses");
    dat.fkTrademarkExaminer = rec.KeyField("fkTrademarkExaminer");
    dat.fkFilingBasis = rec.KeyField("fkFilingBasis");
    dat.fkTrademarkJurisdiction = rec.KeyField("fkTrademarkJurisdiction");
    dat.fkSpecimen = rec.KeyField("fkSpecimen");
    dat.fkEvidenceOfUse = rec.KeyField("fkEvidenceOfUse");
    dat.Mark = rec.StringField("Mark");
    dat.GoodsServices = rec.StringField("GoodsServices");
    dat.fkOwner = rec.KeyField("Owner");

    return dat;
}


QVariant Nutdb::NullableInteger(Key value)
{
    if (value == 0)
        return QVariant();
    else
        return QVariant::fromValue(value);
}

QVariant Nutdb::NullableDate(const QDate date)
{
    if (date.isNull())
    {
        return QVariant();
    }
    else if (date <= QDate::fromString("2000-01-01"))
    {
        return QVariant();
    }
    else
    {
        return QVariant(date);
    }
}

bool Nutdb::UpdateAppointment(AppointmentData dat)
{
    QVariantList params;
    params.append(QVariant::fromValue(dat.AppointmentId));
    params.append(QVariant(dat.AppointmentTime));
    params.append(NullableInteger(dat.fkAppointmentType));

    CallStoredProcedure("UpdateAppointment", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateCopyrightMatter(CopyrightMatterData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.CopyrightMatterId));
    params.append(NullableInteger(dat.fkAuthor));
    params.append(NullableDate(dat.Created));
    params.append(NullableDate(dat.Filed));
    params.append(NullableDate(dat.Registered));
    params.append(QVariant(dat.SerialNumber));
    params.append(QVariant(dat.RegistrationNumber));
    params.append(NullableInteger(dat.fkDeposit));
    params.append(NullableInteger(dat.fkWorkType));

    CallStoredProcedure("UpdateCopyrightMatter", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateCopyrightFiling(CopyrightFilingData dat)
{
    Q_UNUSED(dat);
    // No fields to update, so just return false
    Logger::LogMessage("Tried to update a copyrightMatter, but this table has no updateable fields.");
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateDeadline(DeadlineData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.DeadlineId));

    params.append(NullableDate(dat.TriggerDate));
    params.append(NullableDate(dat.SoftDeadline));
    params.append(NullableDate(dat.HardDeadline));
    params.append(NullableDate(dat.NextDeadline));

    CallStoredProcedure("UpdateDeadline", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateDocument(DocumentData dat)
{
    QVariantList params;
    params.append(QVariant::fromValue(dat.DocumentId));
    params.append(dat.FullyQualifiedPath);
    params.append(dat.URL);
    params.append(dat.Filename);
    params.append(dat.Extension);
    params.append(dat.Title);

    CallStoredProcedure("UpdateDocument", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateEnterprise(EnterpriseData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.EnterpriseId));
    params.append(QVariant(dat.EnterpriseName));
    params.append(NullableInteger(dat.fkBusinessOrganizationType));
    params.append(NullableInteger(dat.fkStateOfIncorporation));
    params.append(NullableInteger(dat.fkMainContact));
    params.append(NullableInteger(dat.OldOrganizationId));

    CallStoredProcedure("UpdateEnterprise", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateEntity(EntityData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.EntityId));
    params.append(QVariant(dat.EntityName));
    params.append(QVariant(dat.FullLegalName));
    params.append(QVariant(dat.PrimaryAddress));
    params.append(QVariant(dat.SecondaryAddress));
    params.append(QVariant(dat.Website));
    params.append(QVariant(dat.PrimaryPhone));
    params.append(QVariant(dat.SecondaryPhone));
    params.append(QVariant(dat.FaxNumber));
    params.append(QVariant(dat.PrimaryEmail));
    params.append(QVariant(dat.SecondaryEmail));
    params.append(NullableInteger(dat.fkState));
    params.append(NullableInteger(dat.fkJurisdiction));
    params.append(dat.DocketPrefix);

    CallStoredProcedure("UpdateEntity", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateFiling(FilingData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.FilingId));
    params.append(NullableInteger(dat.fkFilingStatus));
    params.append(NullableInteger(dat.fkAsFiledDocument));

    CallStoredProcedure("UpdateFiling", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateGeneralMatter(GeneralMatterData dat)
{
    Q_UNUSED(dat);
    Logger::LogMessage("Called UpdateGeneralCase, but there are no fields to update.");
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateGeneralTask(GeneralTaskData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.GeneralTaskId));
    params.append(QVariant(dat.Title));
    params.append(QVariant(dat.Description));
    params.append(NullableInteger(dat.fkFiledDocument));
    params.append(NullableInteger(dat.fkTriggeringDocument));

    CallStoredProcedure("UpdateGeneralTask", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateJurisdiction(JurisdictionData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.JurisdictionId));
    params.append(QVariant(dat.JurisdictionName));
    params.append(QVariant(dat.Description));
    params.append(QVariant(dat.CountryCode));
    params.append(QVariant(dat.TLD));
    params.append(QVariant(dat.ValidResidence));
    params.append(QVariant(dat.ValidCitizenship));

    CallStoredProcedure("UpdateJurisdiction", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateMatter(MatterData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.MatterId));
    params.append(NullableInteger(dat.fkParent));
    params.append(QVariant(dat.AttorneyDocketNumber));
    params.append(QVariant(dat.ClientDocketNumber));
    params.append(QVariant(dat.Title));
    params.append(NullableInteger(dat.fkClient));
    params.append(NullableInteger(dat.fkAssigningFirm));
    params.append(NullableInteger(dat.fkDefaultWorkAttorney));
    params.append(NullableInteger(dat.fkDefaultParalegal));
    params.append(NullableInteger(dat.fkKeyDocument));
    params.append(NullableInteger(dat.fkMatterJurisdiction));
    params.append(NullableInteger(dat.OldMatterId));

    CallStoredProcedure("UpdateMatter", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateNote(NoteData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.NoteId));
    params.append(QVariant(dat.Content));

    CallStoredProcedure("UpdateNote", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateObject(ObjectData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.ObjectId));
    params.append(NullableInteger(dat.fkObjectType));

    CallStoredProcedure("UpdateObject", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdatePatentFiling(PatentFilingData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.PatentFilingId));
    params.append(NullableInteger(dat.fkInventorInterview));

    CallStoredProcedure("UpdatePatentFiling", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdatePatentMatter(PatentMatterData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.PatentMatterId));
    params.append(NullableDate(dat.FilingDate));
    params.append(dat.ApplicationSerialNumber);
    params.append(dat.ConfirmationNumber);
    params.append(dat.ArtUnit);
    params.append(dat.PatentNumber);
    params.append(NullableInteger(dat.fkExaminer));
    params.append(NullableInteger(dat.fkFirstInventor));
    params.append(NullableInteger(dat.fkSupervisoryExaminer));
    params.append(NullableInteger(dat.fkApplicant));
    params.append(NullableDate(dat.BarDate));
    params.append(NullableDate(dat.CriticalDate));
    params.append(NullableDate(dat.DateIssued));
    params.append(NullableInteger(dat.fkSpecification));
    params.append(NullableInteger(dat.fkDrawings));
    params.append(NullableInteger(dat.fkAsFiledClaims));

    CallStoredProcedure("UpdatePatentMatter", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdatePatentResponse(PatentResponseData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.PatentResponseId));
    params.append(NullableInteger(dat.fkOfficeAction));
    params.append(NullableInteger(dat.fkAsFiledResponse));
    params.append(NullableInteger(dat.fkLastFiledResponse));
    params.append(NullableInteger(dat.fkClaimAmendment));

    CallStoredProcedure("UpdatePatentResponse", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdatePerson(PersonData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.PersonId));
    params.append(dat.FirstName);
    params.append(dat.LastName);
    params.append(NullableInteger(dat.fkResidence));
    params.append(NullableInteger(dat.fkCitizenship));
    params.append(NullableInteger(dat.OldId));

    CallStoredProcedure("UpdatePerson", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdatePortfolio(PortfolioData dat)
{
    QVariantList params;

    params.append(NullableInteger(dat.PortfolioId));
    params.append(dat.PortfolioName);
    params.append(NullableInteger(dat.fkClient));

    CallStoredProcedure("UpdatePortfolio", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateResponse(ResponseData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.ResponseId));
    params.append(NullableInteger(dat.fkClientOfficeHours));
    params.append(NullableInteger(dat.fkExaminerInterview));
    params.append(dat.MailingDate);
    params.append(dat.DateFiled);
    params.append(NullableInteger(dat.fkResponseAsFiled));
    params.append(NullableInteger(dat.fkActionDocument));

    CallStoredProcedure("UpdateResponse", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateTag(TagData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.TagId));
    params.append(dat.TagText);

    CallStoredProcedure("UpdateTag", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateTask(TaskData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.TaskId));
    params.append(NullableInteger(dat.fkMatter));
    params.append(NullableDate(dat.DateAssigned));
    params.append(NullableInteger(dat.fkDeadline));
    params.append(NullableInteger(dat.ExpectedFee));
    params.append(NullableInteger(dat.FeePercent));
    params.append(NullableDate(dat.ApprovalRequested));
    params.append(NullableDate(dat.ApprovalReceived));
    params.append(QVariant::fromValue(dat.Complete));
    params.append(NullableInteger(dat.fkWorkAttorney));
    params.append(NullableInteger(dat.fkParalegal));
    params.append(NullableInteger(dat.fkAuthorizationDocument));
    params.append(NullableInteger(dat.OldTaskId));
    params.append(NullableInteger(dat.fkTaskType));

    CallStoredProcedure("UpdateTask", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateTrademarkFiling(TrademarkFilingData dat)
{
    Q_UNUSED(dat);
    Logger::LogMessage("Called UpdateTrademarkFiling, but there are no fields to update.");
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateTrademarkMatter(TrademarkMatterData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.TrademarkMatterId));
    params.append(NullableDate(dat.FirstUseInCommerce));
    params.append(dat.InternationalClass);
    params.append(NullableInteger(dat.fkStatus));
    params.append(dat.SerialNumber);
    params.append(dat.RegistrationNumber);
    params.append(NullableDate(dat.PublicationDate));
    params.append(NullableDate(dat.WindowOpens));
    params.append(NullableDate(dat.NofeeWindowCloses));
    params.append(NullableDate(dat.FinalWindowCloses));
    params.append(NullableInteger(dat.fkTrademarkExaminer));
    params.append(NullableInteger(dat.fkFilingBasis));
    params.append(NullableInteger(dat.fkSpecimen));
    params.append(NullableInteger(dat.fkEvidenceOfUse));
    params.append(dat.Mark);
    params.append(dat.GoodsServices);
    params.append(NullableInteger(dat.fkOwner));

    CallStoredProcedure("UpdateTrademarkMatter", params);
    return mLastOperationSuccessful;
}

bool Nutdb::UpdateTrademarkResponse(TrademarkResponseData dat)
{
    QVariantList params;

    params.append(QVariant::fromValue(dat.TrademarkResponseId));
    params.append(NullableInteger(dat.fkOfficeAction));

    CallStoredProcedure("UpdateTrademarkResponse", params);
    return mLastOperationSuccessful;
}

bool Nutdb::RemoveCaseInventor(Key patentMatterId, Key personId)
{
    QSqlDatabase db = QSqlDatabase::database(); // Assuming you're using the default connection
    QSqlQuery query(db);

    // Start transaction
    if (!db.transaction()) {
        qWarning() << "Failed to start transaction:" << db.lastError().text();
        return false;
    }

    // Prepare the SQL query for deletion
    query.prepare("DELETE FROM patentMatter_inventor WHERE fkPatentMatter = :patentMatterId AND fkPerson = :personId");
    query.bindValue(":patentMatterId", QVariant::fromValue(patentMatterId));
    query.bindValue(":personId", QVariant::fromValue(personId));

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Failed to execute delete query:" << query.lastError().text();
        db.rollback(); // Rollback transaction if there's an error
        return false;
    }

    // Check if any rows were affected
    if (query.numRowsAffected() == 0) {
        qWarning() << "No rows were affected; the inventor might not have been associated with the case.";
    }

    // Commit transaction
    if (!db.commit()) {
        qWarning() << "Failed to commit transaction:" << db.lastError().text();
        return false;
    }

    // Operation was successful
    return true;
}

bool Nutdb::RemoveRoleFromPerson(Key personId, Key roleId)
{
    QVariantList params;
    params.append(QVariant::fromValue(personId));
    params.append(QVariant::fromValue(roleId));

    CallStoredProcedure("RemoveRoleFromPerson", params);
    return mLastOperationSuccessful;
}

bool Nutdb::RemoveRoleFromEntity(Key entityId, Key roleId)
{
    QVariantList params;

    params.append(QVariant::fromValue(entityId));
    params.append(QVariant::fromValue(roleId));

    CallStoredProcedure("RemoveRoleFromEntity", params);
    return mLastOperationSuccessful;
}

Key Nutdb::AssignCaseInventor(Key personId, Key patentCaseId)
{
    QSqlDatabase db = QSqlDatabase::database(); // Using the default connection
    QSqlQuery query(db);

    Key insertedId = 0; // Default to 0 if no insertion happens

    // Begin transaction
    if (!db.transaction()) {
        qWarning() << "Transaction start failed:" << db.lastError().text();
        return insertedId;
    }

    // Check if there's already a matching entry
    query.prepare("SELECT patentMatter_inventorId FROM patentMatter_inventor WHERE fkPerson = :personId AND fkPatentMatter = :patentCaseId");
    query.bindValue(":personId", QVariant::fromValue(personId));
    query.bindValue(":patentCaseId", QVariant::fromValue(patentCaseId));

    if (!query.exec()) {
        qWarning() << "Query failed:" << query.lastError().text();
        db.rollback();
        return insertedId;
    }

    if (query.next()) {
        // Matching entry found, return the existing ID
        insertedId = query.value(0).toUInt();
    } else {
        // No matching entry, insert a new one
        query.prepare("INSERT INTO patentMatter_inventor (fkPerson, fkPatentMatter) VALUES (:personId, :patentCaseId)");
        query.bindValue(":personId", QVariant::fromValue(personId));
        query.bindValue(":patentCaseId", QVariant::fromValue(patentCaseId));

        if (!query.exec()) {
            qWarning() << "Insert failed:" << query.lastError().text();
            db.rollback();
            return insertedId;
        }

        // Get the last inserted ID
        insertedId = query.lastInsertId().toUInt();

        Logger::LogMessage(QString("Assigned person %1 as an inventor to case %2 and got insert ID %3.")
                .arg(personId).arg(patentCaseId).arg(insertedId));
    }

    // Commit transaction
    if (!db.commit()) {
        qWarning() << "Commit failed:" << db.lastError().text();
        return insertedId;
    }

    return insertedId;
}

Key Nutdb::AssignMatterParalegal(Key matterId, Key personId)
{
    QVariantList params;

    params.append(QVariant::fromValue(matterId));
    params.append(QVariant::fromValue(personId));

    return CallStoredKeyProcedure("AssignMatterParalegal", params);
}

Key Nutdb::AssignPatentCaseExaminer(Key patentCaseId, Key personId)
{
    QVariantList params;

    params.append(QVariant::fromValue(patentCaseId));
    params.append(QVariant::fromValue(personId));

    return CallStoredKeyProcedure("AssignPatentCaseExaminer", params);
}

Key Nutdb::AssignTrademarkCaseExaminer(Key trademarkCaseId, Key personId)
{
    QVariantList params;

    params.append(QVariant::fromValue(trademarkCaseId));
    params.append(QVariant::fromValue(personId));

    return CallStoredKeyProcedure("AssignTrademarkCaseExaminer", params);
}

Key Nutdb::AssociateAppointmentWithObject(Key appointmentId, Key objectId)
{
    QVariantList params;

    params.append(QVariant::fromValue(objectId));
    params.append(QVariant::fromValue(appointmentId));

    return CallStoredKeyProcedure("AssociateAppointmentWithObject", params);
}

Key Nutdb::AssociateDocumentWithObject(Key documentId, Key objectId)
{
    QVariantList params;

    params.append(QVariant::fromValue(objectId));
    params.append(QVariant::fromValue(documentId));

    return CallStoredKeyProcedure("AssocaiteDocumentWithObject", params);
}

Key Nutdb::ClearFlag(Key associatedObjectId, QString camelCase)
{
    QVariantList params;

    params.append(QVariant::fromValue(associatedObjectId));
    params.append(camelCase);

    return CallStoredKeyProcedure("ClearFlag", params);
}

Key Nutdb::ClearTag(Key objectId, QString tagText)
{
    QVariantList params;

    params.append(QVariant::fromValue(objectId));
    params.append(tagText);

    return CallStoredProcedure("ClearTag", params).isActive();
}

Key Nutdb::InsertAppointment(DateTime appointmentTime, Key taskId)
{
    QVariantList params;

    params.append(QVariant::fromValue(taskId));
    params.append(appointmentTime);

    return CallStoredKeyProcedure("InsertAppointment", params);
}

Key Nutdb::InsertAppointment(DateTime appointmentTime)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    QVariantList params;
    params.append("appointment");
    Key newobj = CallStoredKeyProcedure("InsertObject", params);

    // If call was successful and returned a new key, populate the new appointment
    if(newobj)
    {
        query.prepare("INSERT INTO appointment (AppointmentId, AppointmentTime) VALUES (:id, :time)");
        query.bindValue(":id", QVariant::fromValue(newobj));
        query.bindValue(":time", appointmentTime);

        if (query.exec()) {
            // Check if the insert was successful by checking affected rows
            if (query.numRowsAffected() > 0) {
                return newobj; // Return the new key as it was the last inserted id
            } else {
                return 0; // Insert failed but no specific error handling here
            }
        } else {
            // Log or handle the error from the query
            qDebug() << "Error inserting appointment:" << query.lastError().text();
            return 0;
        }
    }
    else {
        // If CallStoredKeyProcedure failed or returned an invalid Key
        return 0;
    }
}

Key Nutdb::InsertAppointmentWithZone(DateTime appointmentTime, int utcOffset, Key taskId)
{
    QVariantList params;

    params.append(QVariant::fromValue(taskId));
    params.append(appointmentTime);
    params.append(QVariant::fromValue(utcOffset));

    return CallStoredKeyProcedure("InsertAppointmentWithZone", params);
}

Key Nutdb::InsertAppointmentWithZone(DateTime appointmentTime, int utcOffset)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    QVariantList params;
    params.append("appointment");
    Key newobj = CallStoredKeyProcedure("InsertObject", params);

    // If call was successful and returned a new key, populate the new appointment
    if(newobj)
    {
        // Convert local time to UTC using utcOffset
        QDateTime utcTime = appointmentTime.addSecs(-utcOffset * 3600); // Convert to UTC by subtracting the offset in seconds

        query.prepare("INSERT INTO appointment (AppointmentId, AppointmentTime) VALUES (:id, :time)");
        query.bindValue(":id", QVariant::fromValue(newobj));
        query.bindValue(":time", utcTime); // Now binding UTC time

        if (query.exec()) {
            // Check if the insert was successful by checking affected rows
            if (query.numRowsAffected() > 0) {
                return newobj; // Return the new key as it was the last inserted id
            } else {
                return 0; // Insert failed but no specific error handling here
            }
        } else {
            // Log or handle the error from the query
            qDebug() << "Error inserting appointment:" << query.lastError().text();
            return 0;
        }
    }
    else {
        // If CallStoredKeyProcedure failed or returned an invalid Key
        return 0;
    }
}

Key Nutdb::InsertCaseInventor(QString firstName, QString lastName, Key patentCaseId)
{
    QVariantList params;

    params.append(firstName);
    params.append(lastName);
    params.append(QVariant::fromValue(patentCaseId));

    return CallStoredKeyProcedure("InsertCaseInventor", params);
}

Key Nutdb::InsertClientEnterprise(QString enterpriseName)
{
    QVariantList params;

    params.append(enterpriseName);

    return CallStoredKeyProcedure("InsertClientEnterprise", params);
}

QString Nutdb::GetPrimaryKeyName(QString table)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QString primaryKey;

    // Prepare the SQL query
    QString sql = QString("SHOW KEYS FROM %1 WHERE Key_name = 'PRIMARY'").arg(table);
    query.prepare(sql);

    // Execute the query
    mLastOperationSuccessful = query.exec();
    if (mLastOperationSuccessful)
    {
        if (query.next())
        {
            // Retrieve the primary key column name
            primaryKey = query.value("Column_name").toString(); // Use correct case
        }
        else
        {
            // No primary key found
            QString msg = QString("Could not find a primary key column for table %1.")
                              .arg(table);
            Nutdb::AddErrorMessage(msg);
            return QString(); // Assuming QString is compatible with an empty return
        }
    }
    else
    {
        // Query execution failed
        QString msg = QString("Error executing query: '%1'.").arg(sql);
        Nutdb::AddErrorMessage(msg);
        return QString(); // Assuming QString is compatible with an empty return
    }

    return primaryKey;
}


} // namespace Nutmeg
