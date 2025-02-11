#include "models.h"
#include "dbaccess/taskmodel.h"
#include "dbaccess/viewfilingtypesmodel.h"

namespace Nutmeg{

std::unique_ptr<appointmentTypeModel> gAppointmentTypeModel;
std::unique_ptr<deadlineModel> gDeadlineModel;
std::unique_ptr<flagClassModel> gFlagClassModel;
std::unique_ptr<objectModel> gObjectModel;
std::unique_ptr<objectTypeModel> gObjectTypeModel;
std::unique_ptr<tagModel> gTagModel;
std::unique_ptr<taskModel> gTaskModel;
std::unique_ptr<taskTypeModel> gTaskTypeModel;
std::unique_ptr<viewAppointmentObjectsModel>gViewAppointmentObjectsModel;
std::unique_ptr<viewClientsModel> gViewClientsModel;
std::unique_ptr<viewContractingFirmsModel> gViewContractingFirmsModel;
std::unique_ptr<viewCopyrightFilingTypesModel> gViewCopyrightFilingTypesModel;
std::unique_ptr<viewCopyrightTaskTypesModel> gViewCopyrightTaskTypesModel;
std::unique_ptr<viewEntitiesModel> gViewEntitiesModel;
std::unique_ptr<viewFilingsModel> gViewFilingsModel;
std::unique_ptr<viewFilingsIncompleteModel> gViewFilingsIncompleteModel;
std::unique_ptr<viewFilingTaskTypesModel> gViewFilingTaskTypesModel;
std::unique_ptr<viewFilingTypesModel> gViewFilingTypesModel;
std::unique_ptr<viewInventorsModel> gViewInventorsModel;
std::unique_ptr<viewJurisdictionsModel> gViewJurisdictionsModel;
std::unique_ptr<viewMattersModel> gViewMattersModel;
std::unique_ptr<viewObjectFlagsModel> gViewObjectFlagsModel;
std::unique_ptr<viewObjectTagsModel> gViewObjectTagsModel;
std::unique_ptr<viewParalegalsModel> gViewParalegalsModel;
std::unique_ptr<viewPatentExaminersModel> gViewPatentExaminersModel;
std::unique_ptr<viewPatentFilingTypesModel> gViewPatentFilingTypesModel;
std::unique_ptr<viewPatentMattersModel> gViewPatentMattersModel;
std::unique_ptr<viewPatentTaskTypesModel> gViewPatentTaskTypesModel;
std::unique_ptr<viewPeopleModel> gViewPeopleModel;
std::unique_ptr<viewResponsesModel> gViewResponsesModel;
std::unique_ptr<viewResponsesIncompleteModel> gViewResponsesIncompleteModel;
std::unique_ptr<viewResponseTaskTypesModel> gViewResponseTaskTypesModel;
std::unique_ptr<viewTasksModel> gViewTasksModel;
std::unique_ptr<viewTaskClassModel> gViewTaskClassModel;
std::unique_ptr<viewTaskTypesModel> gViewTaskTypesModel;
std::unique_ptr<viewTrademarkFilingTypesModel> gViewTrademarkFilingTypesModel;
std::unique_ptr<viewTrademarkMattersModel> gViewTrademarkMattersModel;
std::unique_ptr<viewTrademarkTaskTypesModel> gViewTrademarkTaskTypesModel;
std::unique_ptr<viewWorkAttorneysModel> gViewWorkAttorneysModel;

} //namespace Nutmeg
