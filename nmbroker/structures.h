#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "nutmeg.h"

namespace Nutmeg
{

typedef struct
{
    Key ActionId;
    QString ActionName;
} ActionData;

typedef struct
{
    Key AppointmentId;
    QDateTime AppointmentTime;
    Key fkAppointmentType;
} AppointmentData;

typedef struct
{
    Key CopyrightFilingId;
} CopyrightFilingData;

typedef struct
{
    Key CopyrightFormId;
    QString FormName;
    Key fkStoredForm;
} CopyrightFormData;

typedef struct
{
    Key CopyrightMatterId;
    Key fkAuthor;
    Date Created;
    Date Filed;
    Date Registered;
    QString SerialNumber;
    QString RegistrationNumber;
    Key fkDeposit;
    Key fkWorkType;
} CopyrightMatterData;

typedef struct
{
    Key DeadlineId;
    Date TriggerDate;
    Date SoftDeadline;
    Date HardDeadline;
    Date NextDeadline;
} DeadlineData;

typedef struct
{
    Key DocumentId;
    QString FullyQualifiedPath;
    QString URL;
    QString Filename;
    QString Extension;
    ByteVector FileContent;
    QString Title;
} DocumentData;

typedef struct
{
    Key EnterpriseId;
    QString EnterpriseName;
    Key fkBusinessOrganizationType;
    Key fkBusinessJurisdiction;
    Key fkStateOfIncorporation;
    Key fkMainContact;
    Key OldOrganizationId;
} EnterpriseData;

typedef struct
{
    Key EntityId;
    QString EntityName;
    QString FullLegalName;
    QString PrimaryAddress;
    QString SecondaryAddress;
    QString Website;
    QString PrimaryPhone;
    QString SecondaryPhone;
    QString FaxNumber;
    QString PrimaryEmail;
    QString SecondaryEmail;
    Key fkState;
    Key fkJurisdiction;
    QString DocketPrefix;
} EntityData;

typedef struct
{
    Key EnterpriseRoleId;
    QString RoleName;
} EnterpriseRoleData;

typedef struct
{
    Key EntityTypeId;
    QString TypeName;
} EntityTypeData;

typedef struct
{
    Key FilingId;
    Key fkFilingStatus;
    Key fkAsFiledDocument;
} FilingData;

typedef struct
{
    Key FilingBasisId;
    QString Section;
    QString Description;
} FilingBasisData;

typedef struct
{
    Key FilingStatusId;
    QString Status;
    QString Comment;
} FilingStatusData;

typedef struct
{
    Key FlagClassId;
    QString CamelCase;
    QString Label;
    QString Description;
} FlagClassData;

typedef struct
{
    Key GeneralMatterId;
} GeneralMatterData;

typedef struct
{
    Key GeneralTaskId;
    QString Title;
    QString Description;
    Key fkFiledDocument;
    Key fkTriggeringDocument;
} GeneralTaskData;

typedef struct
{
    Key JurisdictionId;
    QString JurisdictionName;
    QString Description;
    QString CountryCode;
    QString TLD;
    bool ValidResidence;
    bool ValidCitizenship;
} JurisdictionData;

typedef struct
{
    Key MatterId;
    Key fkParent;
    QString AttorneyDocketNumber;
    QString ClientDocketNumber;
    QString Title;
    Key fkClient;
    Key fkAssigningFirm;
    Key fkDefaultWorkAttorney;
    Key fkDefaultParalegal;
    Key fkKeyDocument;
    Key fkMatterJurisdiction;
    Key OldMatterId;
} MatterData;

typedef struct
{
    Key NoteId;
    QString Content;
} NoteData;

typedef struct
{
    Key ObjectId;
    Key fkObjectType;
} ObjectData;

typedef struct
{
    Key ObjectTypeId;
    QString TypeName;
} ObjectTypeData;

typedef struct
{
    Key PatentFilingId;
    Key fkInventorInterview;
} PatentFilingData;

typedef struct
{
    Key PatentMatterId;
    Date FilingDate;
    QString ApplicationSerialNumber;
    QString ConfirmationNumber;
    QString ArtUnit;
    QString PatentNumber;
    Key fkExaminer;
    Key fkFirstInventor;
    Key fkSupervisoryExaminer;
    Key fkApplicant;
    Date BarDate;
    Date CriticalDate;
    Date DateIssued;
    Key fkSpecification;
    Key fkDrawings;
    Key fkAsFiledClaims;
} PatentMatterData;

typedef struct
{
    Key PatentResponseId;
    Key fkOfficeAction;
    Key fkAsFiledResponse;
    Key fkLastFiledResponse;
    Key fkClaimAmendment;
} PatentResponseData;

typedef struct
{
    Key PersonId;
    QString FirstName;
    QString LastName;
    Key fkResidence;
    Key fkCitizenship;
    Key OldId;
} PersonData;

typedef struct
{
    Key PortfolioId;
    QString PortfolioName;
    Key fkClient;
} PortfolioData;

typedef struct
{
    Key ResponseId;
    Key fkClientOfficeHours;
    Key fkExaminerInterview;
    Date MailingDate;
    Date DateFiled;
    Key fkResponseAsFiled;
    Key fkActionDocument;
} ResponseData;

typedef struct
{
    Key RoleId;
    QString RoleName;
} RoleData;

typedef struct
{
    Key StateId;
    QString StateName;
    QString Abbreviation;
} StateData;

typedef struct
{
    Key TagId;
    QString TagText;
} TagData;

typedef struct
{
    Key TaskId;
    Key fkMatter;
    Date DateAssigned;
    Key fkDeadline;
    float ExpectedFee;
    float FeePercent;
    Date ApprovalRequested;
    Date ApprovalReceived;
    bool Complete;
    Key fkWorkAttorney;
    Key fkParalegal;
    Key fkAuthorizationDocument;
    Key OldTaskId;
    Key fkTaskType;
} TaskData;

typedef struct
{
    Key TaskClassId;
    QString TaskClassName;
} TaskClassData;

typedef struct
{
    Key TaskTypeId;
    Key fkTaskClass;
    QString TaskName;
} TaskTypeData;

typedef struct
{
    Key TrademarkFilingId;
} TrademarkFilingData;

typedef struct
{
    Key TrademarkMatterId;
    Date FirstUseInCommerce;
    Uint8 InternationalClass;
    Key fkStatus;
    QString SerialNumber;
    QString RegistrationNumber;
    Date PublicationDate;
    Date WindowOpens;
    Date NofeeWindowCloses;
    Date FinalWindowCloses;
    Key fkTrademarkExaminer;
    Key fkFilingBasis;
    Key fkTrademarkJurisdiction;
    Key fkSpecimen;
    Key fkEvidenceOfUse;
    QString Mark;
    QString GoodsServices;
    Key fkOwner;
} TrademarkMatterData;

typedef struct
{
    Key TrademarkResponseId;
    Key fkOfficeAction;
} TrademarkResponseData;

typedef struct
{
    Key TrademarkStatusId;
    QString StatusText;
} TrademarkStatusData;

typedef struct
{
    Key UserId;
    QString Username;
    QString PasswordHash;
    QString LDAPToken;
} UserData;

typedef struct
{
    Key WebroleId;
    QString WebroleName;
} WebroleData;

typedef struct
{
    Key WorkTypeId;
    QString WorkTypeName;
    Key fkCopyrightForm;
} WorkTypeData;

} // end namespace Nutmeg

#endif // STRUCTURES_H
