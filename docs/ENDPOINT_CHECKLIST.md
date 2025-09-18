# Endpoint Coverage Checklist

Purpose
- Track WebSocket coverage for TableModel and ViewModel classes under `nmbroker/dbaccess`.
- Conventions: Tables → read/write; Views → read-only. Action names follow `resource.get|list|create|update|delete` (tables) or `resourceView.get|list` (views).

Tables (read/write)
- [x] appointmentTypeModel → appointmentType.get/list/create/update/delete
- [x] deadlineModel → deadline.get/update/list/create/delete
- [x] flagClassModel → flagClass.get/update/list/create/delete
- [x] objectModel → object.get/update/list/create/delete
- [x] tagModel → tag.get/search/list/create/update/delete
- [x] taskModel → task.get/update/list
- [x] taskTypeModel → taskType.get/list/create/update/delete
- [x] objectTypeModel → objectType.get/list/update/create/delete

Views (read-only)
- Appointment/Object
  - [x] viewAppointmentObjectsModel → appointmentObject.get, appointmentObject.listForObject
- Parties
  - [x] viewClientsModel → client.get/list
  - [x] viewContractingFirmsModel → contractingFirm.get/list
  - [x] viewEntitiesModel → entityView.get/list
  - [x] viewPeopleModel → peopleView.get/list
  - [x] viewParalegalsModel → paralegalView.get/list
  - [x] viewPatentExaminersModel → patentExaminerView.get/list
  - [x] viewWorkAttorneysModel → workAttorneyView.get/list
- Filings
  - [x] viewFilingsModel → filingView.get/list
  - [x] viewFilingsIncompleteModel → filingIncompleteView.get/list
  - [x] viewFilingTaskTypesModel → filingTaskType.get/list
  - [x] viewFilingTypesModel → filingType.get/list
  - [x] viewPatentFilingTypesModel → patentFilingTypeView.get/list
  - [x] viewTrademarkFilingTypesModel → trademarkFilingTypeView.get/list
- Matters
  - [x] viewMattersModel → matterView.get/list
  - [x] viewPatentMattersModel → patentMatterView.get/list
  - [x] viewTrademarkMattersModel → trademarkMatterView.get/list
- Responses
  - [x] viewResponsesModel → responseView.get/list
  - [x] viewResponsesIncompleteModel → responsesIncompleteView.get/list
  - [x] viewResponseTaskTypesModel → responseTaskTypeView.get/list
- Tasks
  - [x] viewTasksModel → taskView.get/list
  - [x] viewTaskClassModel → taskClass.get/list
  - [x] viewTaskTypesModel → taskTypeView.get/list
- Flags/Tags
  - [x] viewObjectFlagsModel → objectFlagView.get, objectFlagView.listForObject
  - [x] viewObjectTagsModel → objectTagView.get, objectTagView.listForObject
- Appointments
  - [x] viewUpcomingAppointmentsModel → upcomingAppointmentView.get/list

Dashboards (read-only)
- [x] filingsDashboardComplete → filingsDashboardEntry.get/list
- [x] responsesDashboardComplete → responsesDashboardEntry.get/list

How to add a new endpoint
1) Register in `nmbroker/websocketserver.cpp` via `m_router.registerAction("name", ActionSpec{ ... })`.
2) Validate payload (types, required fields) and return `EBADREQ|ENOTFOUND` consistently.
3) Prefer domain setters; otherwise use `Nutdb::UpdateField` or direct SQL for simple CRUD.
4) Build with Clang: `cd nmbroker && qmake QMAKE_CXX=clang++ QMAKE_CC=clang && make -j`.
5) Document the action in `docs/PROTOCOL.md` (add to the Batch Models section).

