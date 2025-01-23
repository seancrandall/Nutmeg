#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "nutmeg.h"

namespace Nutmeg
{

typedef struct
{
    Key ActionId;
    String ActionName;
} ActionData;

typedef struct
{
    Key AppointmentId;
    DateTime AppointmentTime;
    Key fkAppointmentType;
} AppointmentData;

typedef struct
{
    Key CopyrightFilingId;
} CopyrightFilingData;

typedef struct
{
    Key CopyrightFormId;
    String FormName;
    Key fkStoredForm;
} CopyrightFormData;

typedef struct
{
    Key CopyrightMatterId;
    Key fkAuthor;
    Date Created;
    Date Filed;
    Date Registered;
    String SerialNumber;
    String RegistrationNumber;
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
    String FullyQualifiedPath;
    String URL;
    String Filename;
    String Extension;
    ByteVector FileContent;
    String Title;
} DocumentData;

typedef struct
{
    Key EnterpriseId;
    String EnterpriseName;
    Key fkBusinessOrganizationType;
    Key fkBusinessJurisdiction;
    Key fkStateOfIncorporation;
    Key fkMainContact;
    Key OldOrganizationId;
} EnterpriseData;

typedef struct
{
    Key EntityId;
    String EntityName;
    String FullLegalName;
    String PrimaryAddress;
    String SecondaryAddress;
    String Website;
    String PrimaryPhone;
    String SecondaryPhone;
    String FaxNumber;
    String PrimaryEmail;
    String SecondaryEmail;
    Key fkState;
    Key fkJurisdiction;
    String DocketPrefix;
} EntityData;

typedef struct
{
    Key EnterpriseRoleId;
    String RoleName;
} EnterpriseRoleData;

typedef struct
{
    Key EntityTypeId;
    String TypeName;
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
    String Section;
    String Description;
} FilingBasisData;

typedef struct
{
    Key FilingStatusId;
    String Status;
    String Comment;
} FilingStatusData;

typedef struct
{
    Key FlagClassId;
    String CamelCase;
    String Label;
    String Description;
} FlagClassData;

typedef struct
{
    Key GeneralMatterId;
} GeneralMatterData;

typedef struct
{
    Key GeneralTaskId;
    String Title;
    String Description;
    Key fkFiledDocument;
    Key fkTriggeringDocument;
} GeneralTaskData;

typedef struct
{
    Key JurisdictionId;
    String JurisdictionName;
    String Description;
    String CountryCode;
    String TLD;
    bool ValidResidence;
    bool ValidCitizenship;
} JurisdictionData;

typedef struct
{
    Key MatterId;
    Key fkParent;
    String AttorneyDocketNumber;
    String ClientDocketNumber;
    String Title;
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
    String Content;
} NoteData;

typedef struct
{
    Key ObjectId;
    Key fkObjectType;
} ObjectData;

typedef struct
{
    Key ObjectTypeId;
    String TypeNam;
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
    String ApplicationSerialNumber;
    String ConfirmationNumber;
    String ArtUnit;
    String PatentNumber;
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
    String FirstName;
    String LastName;
    Key fkResidence;
    Key fkCitizenship;
    Key OldId;
} PersonData;

typedef struct
{
    Key PortfolioId;
    String PortfolioName;
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
    String RoleName;
} RoleData;

typedef struct
{
    Key StateId;
    String StateName;
    String Abbreviation;
} StateData;

typedef struct
{
    Key TagId;
    String TagText;
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
    String TaskClassName;
} TaskClassData;

typedef struct
{
    Key TaskTypeId;
    Key fkTaskClass;
    String TaskName;
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
    String SerialNumber;
    String RegistrationNumber;
    Date PublicationDate;
    Date WindowOpens;
    Date NofeeWindowCloses;
    Date FinalWindowCloses;
    Key fkTrademarkExaminer;
    Key fkFilingBasis;
    Key fkTrademarkJurisdiction;
    Key fkSpecimen;
    Key fkEvidenceOfUse;
    String Mark;
    String GoodsServices;
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
    String StatusText;
} TrademarkStatusData;

typedef struct
{
    Key UserId;
    String Username;
    String PasswordHash;
    String LDAPToken;
} UserData;

typedef struct
{
    Key WebroleId;
    String WebroleName;
} WebroleData;

typedef struct
{
    Key WorkTypeId;
    String WorkTypeName;
    Key fkCopyrightForm;
} WorkTypeData;

} // end namespace Nutmeg

#endif // STRUCTURES_H
