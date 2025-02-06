#ifndef MODELS_H
#define MODELS_H

#include "appointmenttypemodel.h"
#include "responsesdashboardtablemodel.h"
#include "tablemodel.h"
#include "tasktypemodel.h"
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
#include "viewparalegalsmodel.h"
#include "viewpatentexaminersmodel.h"
#include "viewpatentfilingtypesmodel.h"
#include "viewpatenttasktypesmodel.h"
#include "viewpeoplemodel.h"
#include "viewresponsesmodel.h"
#include "viewresponsesincompletemodel.h"
#include "viewresponsetasktypesmodel.h"
#include "viewtasksmodel.h"
#include "viewtaskclassmodel.h"
#include "viewtasktypesmodel.h"
#include "viewtrademarkfilingtypesmodel.h"
#include "viewtrademarktasktypesmodel.h"
#include "viewworkattorneysmodel.h"

namespace Nutmeg{
extern std::unique_ptr<Nutmeg::appointmentTypeModel> gAppointmentTypeModel;
extern std::unique_ptr<Nutmeg::ResponsesDashboardTableModel> gResponsesDashboardTableModel;
extern std::unique_ptr<Nutmeg::taskTypeModel> gTaskTypeModel;
extern std::unique_ptr<Nutmeg::viewClientsModel> gViewClientsModel;
extern std::unique_ptr<Nutmeg::viewContractingFirmsModel> gViewContractingFirmsModel;
extern std::unique_ptr<Nutmeg::viewFilingsModel> gViewFilingsModel;
extern std::unique_ptr<Nutmeg::viewCopyrightFilingTypesModel> gViewCopyrightFilingTypesModel;
extern std::unique_ptr<Nutmeg::viewCopyrightTaskTypesModel> gViewCopyrightTaskTypesModel;
extern std::unique_ptr<Nutmeg::viewEntitiesModel> gViewEntitiesModel;
extern std::unique_ptr<Nutmeg::viewFilingsIncompleteModel> gViewFilingsIncompleteModel;
extern std::unique_ptr<Nutmeg::viewFilingTaskTypesModel> gViewFilingTaskTypesModel;
extern std::unique_ptr<Nutmeg::viewInventorsModel> gViewInventorsModel;
extern std::unique_ptr<Nutmeg::viewJurisdictionsModel> gViewJurisdictionsModel;
extern std::unique_ptr<Nutmeg::viewMattersModel> gViewMattersModel;
extern std::unique_ptr<Nutmeg::viewParalegalsModel> gViewParalegalsModel;
extern std::unique_ptr<Nutmeg::viewPatentExaminersModel> gViewPatentExaminersModel;
extern std::unique_ptr<Nutmeg::viewPatentFilingTypesModel> gViewPatentFilingTypesModel;
extern std::unique_ptr<Nutmeg::viewPatentTaskTypesModel> gViewPatentTaskTypesModel;
extern std::unique_ptr<Nutmeg::viewPeopleModel> gViewPeopleModel;
extern std::unique_ptr<Nutmeg::viewResponsesModel> gViewResponsesModel;
extern std::unique_ptr<Nutmeg::viewResponsesIncompleteModel> gViewResponsesIncompleteModel;
extern std::unique_ptr<Nutmeg::viewResponseTaskTypesModel> gViewResponseTaskTypesModel;
extern std::unique_ptr<Nutmeg::viewTaskClassModel> gViewTaskClassModel;
extern std::unique_ptr<Nutmeg::viewTaskTypesModel> gViewTaskTypesModel;
extern std::unique_ptr<Nutmeg::viewTrademarkFilingTypesModel> gViewTrademarkFilingTypesModel;
extern std::unique_ptr<Nutmeg::viewTasksModel> gViewTasksModel;
extern std::unique_ptr<Nutmeg::viewTrademarkTaskTypesModel> gViewTrademarkTaskTypesModel;
extern std::unique_ptr<Nutmeg::viewWorkAttorneysModel> gViewWorkAttorneysModel;

} //namespace Nutmeg

#endif // MODELS_H
