#ifndef MODELS_H
#define MODELS_H

#include "appointmenttypemodel.h"
#include "dbaccess/flagclassmodel.h"
#include "dbaccess/taskmodel.h"
#include "dbaccess/viewfilingtypesmodel.h"
#include "dbaccess/viewpatentmattersmodel.h"
#include "dbaccess/viewtrademarkmattersmodel.h"
#include "dbaccess/viewupcomingappointmentsmodel.h"
#include "deadlinemodel.h"
#include "dbaccess/viewobjectflagsmodel.h"
#include "flagclassmodel.h"
#include "objectmodel.h"
#include "objecttypemodel.h"
#include "tablemodel.h"
#include "tagmodel.h"
#include "tasktypemodel.h"
#include "viewappointmentobjectsmodel.h"
#include "viewclientsmodel.h"
#include "viewcontractingfirmsmodel.h"
#include "viewcopyrightfilingtypesmodel.h"
#include "viewcopyrighttasktypesmodel.h"
#include "viewentitiesmodel.h"
#include "viewfilingsmodel.h"
#include "viewfilingsincompletemodel.h"
#include "viewfilingtasktypesmodel.h"
#include "viewinventorsmodel.h"
#include "viewjurisdictionsmodel.h"
#include "viewmattersmodel.h"
#include "viewobjecttagsmodel.h"
#include "viewparalegalsmodel.h"
#include "viewpatentexaminersmodel.h"
#include "viewpatentfilingtypesmodel.h"
#include "viewpatenttasktypesmodel.h"
#include "viewpeoplemodel.h"
#include "viewresponsesmodel.h"
#include "viewresponsetasktypesmodel.h"
#include "viewtasksmodel.h"
#include "viewtaskclassmodel.h"
#include "viewtasktypesmodel.h"
#include "viewtrademarkfilingtypesmodel.h"
#include "viewtrademarktasktypesmodel.h"
#include "viewworkattorneysmodel.h"
#include "responsesdashboardcomplete.h"

namespace Nutmeg{
extern std::unique_ptr<appointmentTypeModel> gAppointmentTypeModel;
extern std::unique_ptr<deadlineModel> gDeadlineModel;
extern std::unique_ptr<flagClassModel> gFlagClassModel;
extern std::unique_ptr<objectModel> gObjectModel;
extern std::unique_ptr<objectTypeModel> gObjectTypeModel;
extern std::unique_ptr<tagModel> gTagModel;
extern std::unique_ptr<taskModel> gTaskModel;
extern std::unique_ptr<taskTypeModel> gTaskTypeModel;
extern std::unique_ptr<viewAppointmentObjectsModel>gViewAppointmentObjectsModel;
extern std::unique_ptr<viewClientsModel> gViewClientsModel;
extern std::unique_ptr<viewContractingFirmsModel> gViewContractingFirmsModel;
extern std::unique_ptr<viewFilingsModel> gViewFilingsModel;
extern std::unique_ptr<viewCopyrightFilingTypesModel> gViewCopyrightFilingTypesModel;
extern std::unique_ptr<viewCopyrightTaskTypesModel> gViewCopyrightTaskTypesModel;
extern std::unique_ptr<viewEntitiesModel> gViewEntitiesModel;
extern std::unique_ptr<viewFilingsIncompleteModel> gViewFilingsIncompleteModel;
extern std::unique_ptr<viewFilingTaskTypesModel> gViewFilingTaskTypesModel;
extern std::unique_ptr<viewFilingTypesModel> gViewFilingTypesModel;
extern std::unique_ptr<viewInventorsModel> gViewInventorsModel;
extern std::unique_ptr<viewJurisdictionsModel> gViewJurisdictionsModel;
extern std::unique_ptr<viewMattersModel> gViewMattersModel;
extern std::unique_ptr<viewObjectFlagsModel> gViewObjectFlagsModel;
extern std::unique_ptr<viewObjectTagsModel> gViewObjectTagsModel;
extern std::unique_ptr<viewParalegalsModel> gViewParalegalsModel;
extern std::unique_ptr<viewPatentExaminersModel> gViewPatentExaminersModel;
extern std::unique_ptr<viewPatentMattersModel> gViewPatentMattersModel;
extern std::unique_ptr<viewPatentFilingTypesModel> gViewPatentFilingTypesModel;
extern std::unique_ptr<viewPatentTaskTypesModel> gViewPatentTaskTypesModel;
extern std::unique_ptr<viewPeopleModel> gViewPeopleModel;
extern std::unique_ptr<viewResponsesModel> gViewResponsesModel;
extern std::unique_ptr<viewResponseTaskTypesModel> gViewResponseTaskTypesModel;
extern std::unique_ptr<viewTaskClassModel> gViewTaskClassModel;
extern std::unique_ptr<viewTaskTypesModel> gViewTaskTypesModel;
extern std::unique_ptr<viewTrademarkFilingTypesModel> gViewTrademarkFilingTypesModel;
extern std::unique_ptr<viewTrademarkMattersModel> gViewTrademarkMattersModel;
extern std::unique_ptr<viewTasksModel> gViewTasksModel;
extern std::unique_ptr<viewTrademarkTaskTypesModel> gViewTrademarkTaskTypesModel;
extern std::unique_ptr<viewUpcomingAppointmentsModel> gViewUpcomingAppointmentsModel;
extern std::unique_ptr<viewWorkAttorneysModel> gViewWorkAttorneysModel;
extern std::unique_ptr<ResponsesDashboardComplete> gResponsesDashboardComplete;

} //namespace Nutmeg

#endif // MODELS_H
