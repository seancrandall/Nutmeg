#ifndef Nutdb_H
#define Nutdb_H

#include <QDebug>
#include <QSetIterator>
#include <QSqlQueryModel>
#include <QSqlResult>
#include <QString>
#include <QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>
#include <QtSql/qsqlerror.h>

#include "nutmeg.h"
#include "property.h"
#include "structures.h"
#include "logger.h"

#define StringField(x) field(x).value().toString()
#define UIntField(x) field(x).value().toUInt()
#define IntField(x) field(x).value().toInt()
#define KeyField(x) field(x).value().toUInt()
#define DateField(x) field(x).value().toDate()
#define DateTimeField(x) field(x).value().toDateTime()
#define FloatField(x) field(x).value().toFloat()
#define BoolField(x) field(x).value().toBool()

namespace Nutmeg
{

class Nutdb
{
  public:
    Nutdb();
    ~Nutdb();

    static void AddErrorMessage(const QString msg);

    static QVariant NullableInteger(Key value);
    static QVariant NullableDate(const QDate date);

    // Database Setup
    // Could do these with public members, but using properties is more long-term
    // maintainable
    Property(getServer, setServer) String server;
    Property(getDatabaseName, setDatabaseName) String databasename;
    Property(getPort, setPort) uint port;
    Property(getUsername, setUsername) String username;
    Property(getPassword, setPassword) String password;
	//static ReadOnlyProperty(getLastOperationSuccessful) bool lastOperationSuccessful;
	ReadOnlyProperty(getLastOperationSuccessful) bool lastOperationSuccessful;

    void setServer(String val) { mServer = val; }
    void setDatabaseName(String val) { mDatabaseName = val; }
    void setPort(uint val) { mPort = val; }
    void setUsername(String val) { mUsername = val; }
    void setPassword(String val) { mPassword = val; }
    String getServer(void) { return mServer; }
    String getDatabaseName(void) { return mDatabaseName; }
    uint getPort(void) { return mPort; }
    String getUsername(void) { return mUsername; }
    String getPassword(void) { return mPassword; }

    // QSqlDatabase& getDb(void){return mDb;}

    bool connect(void);

    // const QSqlQuery& getQuery(void){return mQuery;}

    static QSqlRecord GetRecord(String table, Key id);

    static const QList<QSqlRecord> SearchLike(String table, String fieldName, String searchString);

    // UpdateRoutines
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief UpdateField
    /// \param table
    /// \param field
    /// \param key
    /// \param value
    /// \return
    static bool UpdateField(QString table, QString field, Key key, QString value);
    static bool UpdateAppointment(AppointmentData dat);
    static bool UpdateCopyrightMatter(CopyrightMatterData dat);
    static bool UpdateCopyrightFiling(CopyrightFilingData dat);
    static bool UpdateDeadline(DeadlineData dat);
    static bool UpdateDocument(DocumentData dat);
    static bool UpdateEnterprise(EnterpriseData dat);
    static bool UpdateEntity(EntityData dat);
    static bool UpdateFiling(FilingData dat);
    static bool UpdateGeneralMatter(GeneralMatterData dat);
    static bool UpdateGeneralTask(GeneralTaskData dat);
    static bool UpdateJurisdiction(JurisdictionData dat);
    static bool UpdateMatter(MatterData dat);
    static bool UpdateNote(NoteData dat);
    static bool UpdateObject(ObjectData dat);
    static bool UpdatePatentFiling(PatentFilingData dat);
    static bool UpdatePatentMatter(PatentMatterData dat);
    static bool UpdatePatentResponse(PatentResponseData dat);
    static bool UpdatePerson(PersonData dat);
    static bool UpdatePortfolio(PortfolioData dat);
    static bool UpdateResponse(ResponseData dat);
    static bool UpdateRole(RoleData dat);
    static bool UpdateState(StateData dat);
    static bool UpdateTag(TagData dat);
    static bool UpdateTask(TaskData dat);
    static bool UpdateTrademarkFiling(TrademarkFilingData dat);
    static bool UpdateTrademarkMatter(TrademarkMatterData dat);
    static bool UpdateTrademarkResponse(TrademarkResponseData dat);
    static bool UpdateUser(UserData dat);

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // Insert routines
    static bool RemoveCaseInventor(Key patentMatterId, Key personId);
    static bool RemoveRoleFromPerson(Key personId, Key roleId);
    static bool RemoveRoleFromEntity(Key entityId, Key roleId);
    static Key AddRoleToEntity(Key entityId, Key roleId);
    static Key AddRoleToPerson(Key personId, Key roleId);
    static Key AssignCaseInventor(Key personId, Key patentCaseId);
    static Key AssignMatterParalegal(Key matterId, Key personId);
    static Key AssignPatentCaseExaminer(Key patentCaseId, Key personId);
    static Key AssignTrademarkCaseExaminer(Key trademarkCaseId, Key personId);
    static Key AssociateAppointmentWithObject(Key appointmentId, Key objectId);
    static Key AssociateDocumentWithObject(Key documentId, Key objectId);
    static Key ClearFlag(Key associatedObjectId, String camelCase);
    static Key ClearTag(Key objectId, String tagText);
    static Key InsertAppointment(DateTime appointmentTime, Key taskId);
    static Key InsertAppointment(DateTime appointmentTime);
    static Key InsertAppointmentWithZone(DateTime appointmentTime, int utcOffset, Key taskId);
    static Key InsertAppointmentWithZone(DateTime appointmentTime, int utcOffset);
    static Key InsertCaseInventor(String firstName, String lastName, Key patentCaseId);
    static Key InsertClientEnterprise(String enterpriseName);
    static Key InsertClientNatural(String firstName, String lastName);
    static Key InsertCopyrightFiling(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertCopyrightMatter(String docketNumber);
    static Key InsertDocument(String Title);
    static Key InsertEnterprise(String enterpriseName);
    static Key InsertEntity(String entityName);
    static Key InsertExaminer(String firstName, String lastName);
    static Key InsertFiling(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertFinalOA(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertGeneralMatter(String docketNumber);
    static Key InsertInventor(String firstName, String lastName);
    static Key InsertMatter(String docketNumber);
    static Key InsertNonfinalOA(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertNote(Key objectId, String noteText);
    static Key InsertObject(String objectType);
    static Key InsertParalegal(String firstName, String lastName);
    static Key InsertPatentExaminer(String firstName, String lastName);
    static Key InsertPatentFiling(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertPatentMatter(String docketNumber);
    static Key InsertPatentResponse(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertPerson(String firstName, String lastName);
    static Key InsertResponse(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertTask(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertTaskNinety(Key matterId,
                                Date triggerDate = QDate::currentDate()); /// Inserts a task with a 90-day
                                                                          /// deadline from triggerDate
    static Key InsertTaskSixty(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertTaskThirty(Key matterId, Date triggerDate = QDate::currentDate());
    static Key
    InsertTaskThreeMonth(Key matterId,
                         Date triggerDate = QDate::currentDate()); /// Inserts a task with a 3-month soft deadline
                                                                   /// and 6-month hard deadline. Patent-centric
    static Key InsertTaskThreeMonthHard(Key matterId,
                                        Date triggerDate = QDate::currentDate()); /// Inserts a task with a HARD 3-month
                                                                                  /// deadline (e.g., Issue Fee)
    static Key
    InsertTaskTwoMonth(Key matterId,
                       Date triggerDate = QDate::currentDate()); /// Inserts a task with a 2-month soft deadline
                                                                 /// and 6-month hard deadline. Patent-centric
    static Key InsertTaskWithMatter(String docketNumber, Date triggerDate = QDate::currentDate());
    static Key InsertTrademarkFiling(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertTrademarkMatter(String docketNumber);
    static Key InsertTrademarkResponse(Key matterId, Date triggerDate = QDate::currentDate());
    static Key InsertWorkAttorney(String firstName, String lastName);
    static Key MergeTags(Key tagOneId, Key tagTwoId, String tagText);
    static Key SetFlag(Key associatedObjectId, String camelCase);
    static Key TagObject(Key objectId, String tagText);

    // Get Routines
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    static ActionData GetAction(Key id);
    static AppointmentData GetAppointment(Key id);
    static QList<Key> GetCaseInventors(Key id);
    static QList<Key> GetObjectAppointments(Key id);
    static CopyrightFilingData GetCopyrightFiling(Key id);
    static CopyrightFormData GetCopyrightForm(Key id);
    static CopyrightMatterData GetCopyrightMatter(Key id);
    static DeadlineData GetDeadline(Key id);
    static DocumentData GetDocument(Key id);
    static QList<Key> GetDocumentObjects(Key documentId);
    static QList<Key> GetObjectDocuments(Key objectId);
    static EnterpriseData GetEnterprise(Key id);
    static EntityData GetEntity(Key id);
    static EnterpriseRoleData GetEnterpriseRole(Key id);
    static EntityTypeData GetEntityType(Key id);
    static FilingData GetFiling(Key id);
    static FilingStatusData GetFilingStatus(Key id);
    static bool GetFlag(Key objectId, String camelCase);
    static FlagClassData GetFlagClass(String camelCase);
    static GeneralMatterData GetGeneralMatter(Key id);
    static GeneralTaskData GetGeneralTask(Key id);
    static JurisdictionData GetJurisdiction(Key id);
    static MatterData GetMatter(Key id);
    static NoteData GetNote(Key id);
    static ObjectData GetObject(Key id);
    static QList<String> GetObjectFlags(Key objectId);
    static QList<Key> GetFlagObjects(const QString camelCase);
    static Key GetObjectTypeId(String objectTypeText);
    static String GetObjectTypeString(Key objectTypeId);
    static QList<String> GetObjectTags(Key objectId);
    static QList<Key> GetTagObjects(String tagText);
    static PatentMatterData GetPatentMatter(Key id);
    static PatentResponseData GetPatentResponse(Key id);
    static PersonData GetPerson(Key id);
    static PortfolioData GetPortfolio(Key id);
    static ResponseData GetResponse(Key id);
    static RoleData GetRole(Key id);
    static StateData GetState(Key id);
    static TagData GetTag(Key id);
    static Key GetTagId(String tagText);
    static TaskData GetTask(Key id);
    static TaskClassData GetTaskClas(Key id);
    static TaskTypeData GetTaskType(Key id);
    static TrademarkFilingData GetTrademarkFiling(Key id);
    static TrademarkMatterData GetTrademarkMatter(Key id);
    static TrademarkResponseData GetTrademarkResponse(Key id);
    static TrademarkStatusData GetTrademarkStatus(Key id);
    static UserData GetUser(Key id);
    static WebroleData GetWebrole(Key id);
    static WorkTypeData GetWorkType(Key id);

    static QString EmailInventors(Key PatentCaseId);

    static QVector<FlagClassData> GetFlags(Key objectId);
    static QList<Key> GetInventors(Key patentMatterId);
    static QString FormatParameter(QVariant parameter);

  protected:
    static QSqlQuery
    CallStoredProcedure(QString procName, QVariantList parameters,
                        QString outParamName = QString()); // WARNING: Should only be used with stored procedures
                                                           // that already safe. Performs no parameter binding.
    static QVariant CallStoredReturnProcedure(QString procName, QVariantList parameters);
    static Key CallStoredKeyProcedure(QString procName, QVariantList parameters);
    static Key InsertTag(String tagText); /// InsertTag inserts an unaffiliated tag. Should
                                          /// probably never be used. Instead, use TagObject

    static String GetPrimaryKeyName(QString table);


  private:
    // QSqlDatabase mDb;
    // QSqlQuery *mQuery;

    // Server setup
    String mServer;
    uint mPort = 0;
    String mDatabaseName;
    String mUsername;
    String mPassword;

    static bool mLastOperationSuccessful;
    static uint mErrorCount;
    static QVector<QString> mErrorList;

};

} // namespace Nutmeg

#endif // Nutdb_H
