# Nutmeg WebSocket Protocol (v0)

Scope
- Transport: WebSocket, UTF‑8 JSON text frames only.
- Audience: Frontends (web/desktop) talking to `nmbroker`.
- Stability: Pre‑alpha. Treat as v0; breakage possible. Version field included for evolution.

Envelope
- Request
  - `type`: `"req"`
  - `id`: client-generated string to correlate responses
  - `action`: action name, e.g., `"ping"`, `"matter.get"`
  - `payload`: object with action parameters (may be `{}`)
  - `auth` (optional): `{ "token": "..." }`
  - `version`: protocol version string, e.g., `"0.1"`
- Response
  - `type`: `"res"`
  - `id`: echoes request `id`
  - `ok`: boolean
  - `result` (when `ok=true`): action-specific object or array
  - `error` (when `ok=false`): `{ code, message, details? }`
- Event (server → client notifications)
  - `type`: `"event"`
  - `event`: event name, e.g., `"server.hello"`
  - `payload`: object with event data

Conventions
- Timestamps: ISO‑8601 UTC strings (e.g., `2025-02-28T17:00:00Z`).
- IDs: numeric where possible; encoded as JSON numbers.
- Pagination: requests accept `page { limit, offset }`; responses include `page { limit, offset, total }`.
- Errors: `code` string (stable), human `message`, optional `details` object.
- Versioning: client sends `version`; server may negotiate via `server.hello` event payload.

Examples
- Server hello (sent on connect):
  `{ "type": "event", "event": "server.hello", "payload": { "service": "NutmegBroker", "apiVersion": "0.1" } }`
- Ping
  Request: `{ "type": "req", "id": "1", "action": "ping", "payload": {}, "version": "0.1" }`
  Response: `{ "type": "res", "id": "1", "ok": true, "result": { "pong": true } }`
- Error (unknown action)
  `{ "type": "res", "id": "99", "ok": false, "error": { "code": "ENOACTION", "message": "Unknown action 'foo.bar'" } }`

Initial Actions
- `info.get`
  - Result: `{ service, apiVersion, build? }`
- `ping`
  - Result: `{ pong: true, now: <timestamp> }`
- `matter.get`
  - Payload: `{ id: number }`
  - Result: `{ id, fkParent, attorneyDocketNumber, clientDocketNumber, title, fkClient, fkAssigningFirm, fkDefaultWorkAttorney, fkDefaultParalegal, fkKeyDocument, fkMatterJurisdiction, oldMatterId }`
  - Errors: `ENOTFOUND` if the id does not exist
- `matter.update`
  - Payload: `{ id: number, fkParent?: number, attorneyDocketNumber?: string, clientDocketNumber?: string, title?: string, fkClient?: number, fkAssigningFirm?: number, fkDefaultWorkAttorney?: number, fkDefaultParalegal?: number, fkKeyDocument?: number, fkMatterJurisdiction?: number, oldMatterId?: number }`
  - Result: same shape as `matter.get` (the updated record)
  - Errors: `ENOTFOUND` if the id does not exist; `EBADREQ` for validation/type errors
- `appointment.get`
  - Payload: `{ id: number }`
  - Result: `{ id, appointmentTime: ISO-8601 UTC string, fkAppointmentType, complete, needsAgenda, agendaSent, confirmed, associatedObject, typeString }`
  - Errors: `ENOTFOUND` if the id does not exist
- `appointment.update`
  - Payload: `{ id: number, appointmentTime?: ISO-8601 string, fkAppointmentType?: number, complete?: boolean, needsAgenda?: boolean, agendaSent?: boolean, confirmed?: boolean }`
  - Result: same shape as `appointment.get`
  - Errors: `ENOTFOUND` if the id does not exist; `EBADREQ` if `appointmentTime` is not a valid ISO-8601 string
- `deadline.get`
  - Payload: `{ id: number }`
  - Result: `{ id, triggerDate: YYYY-MM-DD, softDeadline: YYYY-MM-DD, hardDeadline: YYYY-MM-DD, nextDeadline: YYYY-MM-DD, color: "#RRGGBB" }`
  - Notes: `color` encodes urgency derived from `nextDeadline`.
  - Errors: `ENOTFOUND` if the id does not exist
- `deadline.update`
  - Payload: `{ id: number, triggerDate?: YYYY-MM-DD, softDeadline?: YYYY-MM-DD, hardDeadline?: YYYY-MM-DD, nextDeadline?: YYYY-MM-DD }`
  - Result: same shape as `deadline.get`
  - Errors: `ENOTFOUND` if the id does not exist; `EBADREQ` on invalid date strings
- `document.get`
  - Payload: `{ id: number }`
  - Result: `{ id, fullyQualifiedPath, url, filename, extension, title }`
  - Errors: `ENOTFOUND` if the id does not exist
- `document.update`
  - Payload: `{ id: number, fullyQualifiedPath?: string, url?: string, filename?: string, extension?: string, title?: string }`
  - Result: same shape as `document.get`
  - Errors: `ENOTFOUND` if the id does not exist
- `enterprise.get`
  - Payload: `{ id: number }`
  - Result: `{ id, enterpriseName, fkBusinessOrganizationType, fkBusinessJurisdiction, fkStateOfIncorporation, fkMainContact, oldOrganizationId }`
  - Errors: `ENOTFOUND` if the id does not exist
- `enterprise.update`
  - Payload: `{ id: number, enterpriseName?: string, fkBusinessJurisdiction?: number, fkStateOfIncorporation?: number, fkMainContact?: number, oldOrganizationId?: number }`
  - Result: same shape as `enterprise.get`
  - Errors: `ENOTFOUND` if the id does not exist
- `entity.get`
  - Payload: `{ id: number }`
  - Result: `{ id, entityName, fullLegalName, primaryAddress, secondaryAddress, website, primaryPhone, secondaryPhone, faxNumber, primaryEmail, secondaryEmail, fkState, fkJurisdiction, docketPrefix }`
  - Errors: `ENOTFOUND` if the id does not exist
- `entity.update`
  - Payload: `{ id: number, entityName?: string, fullLegalName?: string, primaryAddress?: string, secondaryAddress?: string, website?: string, primaryPhone?: string, secondaryPhone?: string, faxNumber?: string, primaryEmail?: string, secondaryEmail?: string, fkJurisdiction?: number, fkState?: number, docketPrefix?: string }`
  - Result: same shape as `entity.get`
  - Errors: `ENOTFOUND` if the id does not exist; `EBADREQ` reserved for future validation
- `filing.get`
  - Payload: `{ id: number }`
  - Result: `{ id, fkFilingStatus, fkAsFiledDocument }`
  - Errors: `ENOTFOUND` if the id does not exist
- `filing.update`
  - Payload: `{ id: number, fkFilingStatus?: number, fkAsFiledDocument?: number }`
  - Result: same shape as `filing.get`
  - Errors: `ENOTFOUND` if the id does not exist
- `filingsDashboardEntry.get`
  - Payload: `{ taskId: number }`
  - Result: `{ taskId, taskClassName, attorneyDocketNumber, taskName, title, triggerDate: YYYY-MM-DD, nextDeadline: YYYY-MM-DD, softDeadline: YYYY-MM-DD, hardDeadline: YYYY-MM-DD, clientEntityId, clientEntityName, paralegalEntityName, workAttorneyEntityName, withParalegal, needsExaminerInterview, examinerInterviewScheduled }`
  - Errors: `ENOTFOUND` if the row does not exist
- `flag.get`
  - Payload: `{ objectId: number, camelCase: string }`
  - Result: `{ objectId, camelCase, value: boolean }`
- `flag.set`
  - Payload: `{ objectId: number, camelCase: string, value: boolean }`
  - Result: `{ objectId, camelCase, value }`
- `object.get`
  - Payload: `{ id: number }`
  - Result: `{ id, fkObjectType, objectType, flags: [{ flagClassId, camelCase, label, description, value }], tags: [{ tagId, tagText }], documents: number[], appointments: number[] }`
  - Errors: `ENOTFOUND` if the id does not exist
- `object.update`
  - Payload: `{ id: number, fkObjectType?: number, objectType?: string }`
  - Result: same shape as `object.get`
  - Notes: `objectType` changes the textual type and updates `fkObjectType` accordingly.
- `person.get`
  - Payload: `{ id: number }`
  - Result: `{ id, firstName, lastName, fkResidence, fkCitizenship, oldId }`
  - Errors: `ENOTFOUND` if the id does not exist
- `person.update`
  - Payload: `{ id: number, firstName?: string, lastName?: string, fkResidence?: number, fkCitizenship?: number, oldId?: number }`
  - Result: same shape as `person.get`
- `response.get`
  - Payload: `{ id: number }`
  - Result: `{ id, fkClientOfficeHours, fkExaminerInterview, mailingDate: YYYY-MM-DD, dateFiled: YYYY-MM-DD, fkResponseAsFiled, fkActionDocument }`
  - Errors: `ENOTFOUND` if the id does not exist
- `response.update`
  - Payload: `{ id: number, fkClientOfficeHours?: number, fkExaminerInterview?: number, mailingDate?: YYYY-MM-DD, dateFiled?: YYYY-MM-DD, fkResponseAsFiled?: number, fkActionDocument?: number }`
  - Result: same shape as `response.get`
  - Errors: `ENOTFOUND` if the id does not exist; `EBADREQ` on invalid date strings
- `tag.search`
  - Payload: `{ q: string }`
  - Result: `{ items: [{ tagId, tagText }] }`
- `object.tag`
  - Payload: `{ objectId: number, tagText?: string, removeTagText?: string }`
  - Result: `{ objectId, tags: [{ tagId, tagText }] }`
- `task.get`
  - Payload: `{ id: number }`
  - Result: `{ id, fkMatter, dateAssigned: YYYY-MM-DD, fkDeadline, expectedFee, feePercent, approvalRequested: YYYY-MM-DD, approvalReceived: YYYY-MM-DD, complete, fkWorkAttorney, fkParalegal, fkAuthorizationDocument, oldTaskId, fkTaskType, taskTypeString }`
  - Errors: `ENOTFOUND` if the id does not exist
- `task.update`
  - Payload: `{ id: number, fkMatter?: number, dateAssigned?: YYYY-MM-DD, fkDeadline?: number, expectedFee?: number, feePercent?: number, approvalRequested?: YYYY-MM-DD, approvalReceived?: YYYY-MM-DD, complete?: boolean, fkWorkAttorney?: number, fkParalegal?: number, fkAuthorizationDocument?: number, oldTaskId?: number, fkTaskType?: number }`
  - Result: same shape as `task.get`
  - Errors: `ENOTFOUND` or `EBADREQ` for invalid dates
- `trademarkMatter.get`
  - Payload: `{ id: number }`
  - Result: `{ id, firstUseInCommerce: YYYY-MM-DD, internationalClass, fkStatus, serialNumber, registrationNumber, publicationDate: YYYY-MM-DD, windowOpens: YYYY-MM-DD, nofeeWindowCloses: YYYY-MM-DD, finalWindowCloses: YYYY-MM-DD, fkTrademarkExaminer, fkFilingBasis, fkTrademarkJurisdiction, fkSpecimen, fkEvidenceOfUse, mark, goodsServices }`
  - Errors: `ENOTFOUND` if the id does not exist
- `trademarkMatter.update`
  - Payload: `{ id: number, firstUseInCommerce?: YYYY-MM-DD, internationalClass?: number, fkStatus?: number, serialNumber?: string, registrationNumber?: string, publicationDate?: YYYY-MM-DD, windowOpens?: YYYY-MM-DD, nofeeWindowCloses?: YYYY-MM-DD, finalWindowCloses?: YYYY-MM-DD, fkTrademarkExaminer?: number, fkFilingBasis?: number, fkTrademarkJurisdiction?: number, fkSpecimen?: number, fkEvidenceOfUse?: number, mark?: string, goodsServices?: string }`
  - Result: same shape as `trademarkMatter.get`
  - Errors: `ENOTFOUND` or `EBADREQ` for invalid dates
- `tag.get`
  - Payload: `{ id?: number, text?: string }` (at least one required)
  - Result: `{ tagId, tagText }`
  - Errors: `ENOTFOUND` if no match; `EBADREQ` if neither provided
- `taskClass.get`
  - Payload: `{ id: number }`
  - Result: `{ id, taskClassName }`
  - Errors: `ENOTFOUND` if the id does not exist
- `taskType.get`
  - Payload: `{ id: number }`
  - Result: `{ id, fkTaskClass, taskName }`
  - Errors: `ENOTFOUND` if the id does not exist
- `patentMatter.get`
  - Payload: `{ id: number }`
  - Result: `{ id, filingDate: YYYY-MM-DD, applicationSerialNumber, confirmationNumber, artUnit, patentNumber, fkExaminer, fkFirstInventor, fkSupervisoryExaminer, fkApplicant, barDate: YYYY-MM-DD, criticalDate: YYYY-MM-DD, dateIssued: YYYY-MM-DD, fkSpecification, fkDrawings, fkAsFiledClaims }`
  - Errors: `ENOTFOUND` if the id does not exist
Additional Gets
- `patentFiling.get` → `{ id, fkInventorInterview }`
- `patentResponse.get` → `{ id, fkOfficeAction, fkAsFiledResponse, fkLastFiledResponse, fkClaimAmendment }`
- `trademarkFiling.get` → `{ id }`
- `trademarkResponse.get` → `{ id, fkOfficeAction }`
- `patentMatter.update`
  - Payload: `{ id: number, filingDate?: YYYY-MM-DD, applicationSerialNumber?: string, confirmationNumber?: string, artUnit?: string, patentNumber?: string, fkExaminer?: number, fkFirstInventor?: number, fkSupervisoryExaminer?: number, fkApplicant?: number, barDate?: YYYY-MM-DD, criticalDate?: YYYY-MM-DD, dateIssued?: YYYY-MM-DD, fkSpecification?: number, fkDrawings?: number, fkAsFiledClaims?: number }`
  - Result: same shape as `patentMatter.get`
  - Errors: `ENOTFOUND` or `EBADREQ` for invalid dates
- `responsesDashboardEntry.get`
  - Payload: `{ taskId: number }`
  - Result: similar to `filingsDashboardEntry.get` with response-specific fields
  - Errors: `ENOTFOUND` if the row does not exist
- `flagClass.get`
  - Payload: `{ id?: number, camelCase?: string }` (at least one required)
  - Result: `{ id, camelCase, label, description }`
  - Errors: `ENOTFOUND` if no match; `EBADREQ` if neither id nor camelCase provided
- `flagClass.update`
  - Payload: `{ id: number, camelCase?: string, label?: string, description?: string }`
  - Result: same shape as `flagClass.get`
  - Errors: `ENOTFOUND` if id does not exist
- `copyrightMatter.get`
  - Payload: `{ id: number }`
  - Result: `{ id, fkAuthor, created: YYYY-MM-DD, filed: YYYY-MM-DD, registered: YYYY-MM-DD, serialNumber, registrationNumber, fkDeposit, fkWorkType, isRegistered }`
  - Errors: `ENOTFOUND` if the id does not exist
- `copyrightMatter.update`
  - Payload: `{ id: number, fkAuthor?: number, created?: YYYY-MM-DD, filed?: YYYY-MM-DD, registered?: YYYY-MM-DD, serialNumber?: string, registrationNumber?: string, fkDeposit?: number, fkWorkType?: number, isRegistered?: boolean }`
  - Result: same shape as `copyrightMatter.get`
  - Errors: `ENOTFOUND` if the id does not exist; `EBADREQ` on invalid date strings
- Planned
  - `matters.search { q, page? }` → `{ items, page }`
  - `tasks.upcoming { since?, until?, page? }` → `{ items, page }`

Authentication (planned)
- Phase 0: no auth required in trusted environments (LAN/dev).
- Phase 1: `auth.login { username, password }` → `{ token }`; server binds token to socket session.
- Requests may include `auth.token`. Missing/invalid → `EUNAUTHORIZED`.

Error Codes
- `ENOACTION`: unknown action
- `EBADREQ`: invalid payload shape/validation failed
- `ENOTFOUND`: entity not found
- `EUNAUTHORIZED`: missing/invalid auth
- `EINTERNAL`: unexpected error

Examples
- matter.get (ok)
  Request: `{ "type": "req", "id": "42", "action": "matter.get", "payload": { "id": 123 }, "version": "0.1" }`
  Response: `{ "type": "res", "id": "42", "ok": true, "result": { "id": 123, "title": "Sample", ... } }`
- matter.get (not found)
  Response: `{ "type": "res", "id": "42", "ok": false, "error": { "code": "ENOTFOUND", "message": "Matter not found" } }`
- appointment.update (ok)
  Request: `{ "type": "req", "id": "1001", "action": "appointment.update", "payload": { "id": 77, "appointmentTime": "2025-05-01T16:00:00Z", "confirmed": true }, "version": "0.1" }`
  Response: `{ "type": "res", "id": "1001", "ok": true, "result": { "id": 77, "appointmentTime": "2025-05-01T16:00:00Z", ... } }`
  - appointment.get (not found)
  Response: `{ "type": "res", "id": "1002", "ok": false, "error": { "code": "ENOTFOUND", "message": "Appointment not found" } }`

Create Endpoints
- appointment.create
  - Payload: `{ appointmentTime: ISO-8601, taskId?: number, utcOffset?: number }`
  - Result: same shape as `appointment.get`
- caseInventor.create
  - Payload: `{ firstName: string, lastName: string, patentMatterId: number }`
  - Result: `{ id, patentMatterId, firstName, lastName }`
- client.createEnterprise
  - Payload: `{ enterpriseName: string }`
  - Result: client row from `viewClients` when available
- client.createNatural
  - Payload: `{ firstName: string, lastName: string }`
  - Result: client row from `viewClients` when available
- copyrightFiling.create
  - Payload: `{ matterId: number, triggerDate?: YYYY-MM-DD }`
  - Result: `{ id, fkMatter, triggerDate }`
- copyrightMatter.create
  - Payload: `{ docketNumber: string }`
  - Result: `{ id }`
- document.create
  - Payload: `{ title: string }`
  - Result: `{ id, title }`
- enterprise.create
  - Payload: `{ enterpriseName: string }`
  - Result: `{ id, enterpriseName }`
- entity.create
  - Payload: `{ entityName: string }`
  - Result: `{ id, entityName }`
- examiner.create
  - Payload: `{ firstName: string, lastName: string }`
  - Result: person basics `{ id, firstName, lastName }`
- filing.create | finalOA.create | nonfinalOA.create
  - Payload: `{ matterId: number, triggerDate?: YYYY-MM-DD }`
  - Result: `{ id, fkMatter, triggerDate }` (final/nonfinal OA are specific filing types)
- generalMatter.create | matter.create
  - Payload: `{ docketNumber: string }`
  - Result: matter basics `{ id, attorneyDocketNumber?, clientDocketNumber?, title? }`
- inventor.create | paralegal.create | patentExaminer.create | workAttorney.create
  - Payload: `{ firstName: string, lastName: string }`
  - Result: person basics `{ id, firstName, lastName }`
- note.create
  - Payload: `{ objectId: number, noteText: string }`
  - Result: `{ id, content }`
- patentFiling.create | trademarkFiling.create
  - Payload: `{ matterId: number, triggerDate?: YYYY-MM-DD }`
  - Result: `{ id, fkMatter, triggerDate }`
- patentMatter.create | trademarkMatter.create
  - Payload: `{ docketNumber: string }`
  - Result: `{ id }`
- patentResponse.create | trademarkResponse.create | response.create
  - Payload: `{ matterId: number, triggerDate?: YYYY-MM-DD }`
  - Result: `{ id, fkMatter?, triggerDate? }`
- task.create
  - Payload: `{ matterId: number, triggerDate?: YYYY-MM-DD }`
  - Result: `{ id, fkMatter, fkDeadline }`
- taskNinety.create | taskSixty.create | taskThirty.create | taskTwoMonth.create | taskThreeMonth.create | taskThreeMonthHard.create
  - Payload: `{ matterId: number, triggerDate?: YYYY-MM-DD }`
  - Result: `{ id }`
- taskWithMatter.create
  - Payload: `{ docketNumber: string, triggerDate?: YYYY-MM-DD }`
  - Result: `{ id }`

Client Example (JavaScript)
- Minimal browser client that connects, logs `server.hello`, and performs get/update calls.
  ```js
  const ws = new WebSocket("ws://localhost:8787");

  ws.onmessage = (ev) => {
    try { console.log("<-", JSON.parse(ev.data)); }
    catch { console.log("<-", ev.data); }
  };

  ws.onopen = () => {
    // appointment.get
    ws.send(JSON.stringify({
      type: "req",
      id: "get-1",
      action: "appointment.get",
      payload: { id: 77 },
      version: "0.1",
    }));

    // appointment.update
    ws.send(JSON.stringify({
      type: "req",
      id: "upd-1",
      action: "appointment.update",
      payload: { id: 77, confirmed: true },
      version: "0.1",
    }));
  };
  ```


Compatibility with current server
- Current implementation sends a greeting and supports text `ping` → `pong` and echoing messages.
- It will be updated to use the `type=req/res/event` envelope; existing text `ping` will continue to work during transition.


Batch Models — Tables and Views (lightweight)
- Tables are read/write; views are read‑only. Unless stated, list actions return `{ items: [...] }` and get actions return an object representing the row.

- Tables
  - `appointmentType.get|list|create|update|delete`
    - Get/List: `{ id? }` → object/`{ items }`
    - Create: `{ appointmentTypeName }` → created row
    - Update: `{ id, appointmentTypeName? }` → updated row
    - Delete: `{ id }` → `{ id, deleted: true }`
  - `objectType.get|list|update|create|delete`
    - Get/List: by id or all
    - Update: `{ id, objectTypeName? }` → updated row
    - Create: `{ objectTypeName }` → created row
    - Delete: `{ id }` → `{ id, deleted: true }`

- Views (read‑only; each returns full view columns)
  - Appointment/Object mapping
    - `appointmentObject.get` `{ appointmentId }` → `{ appointmentId, objectId, appointmentTypeName }`
    - `appointmentObject.listForObject` `{ objectId }` → `{ objectId, items }`
  - Clients/Firms
    - `client.get|list` (viewClients) — get by `ClientEntityId` or `EntityId`
    - `contractingFirm.get|list` (viewContractingFirms) — get by `ContractingFirmEntityId` or `EntityId`
  - Copyright types
    - `copyrightFilingType.get|list` (viewCopyrightFilingTypes)
    - `copyrightTaskType.get|list` (viewCopyrightTaskTypes)
  - Entities/People/Paralegals/Examiners/Work Attorneys
    - `entityView.get|list` (viewEntities)
    - `peopleView.get|list` (viewPeople)
    - `paralegalView.get|list` (viewParalegals)
    - `patentExaminerView.get|list` (viewPatentExaminers)
    - `workAttorneyView.get|list` (viewWorkAttorneys)
  - Filings
    - `filingView.get|list` (viewFilings)
    - `filingIncompleteView.get|list` (viewFilingsIncomplete)
    - `filingTaskType.get|list` (viewFilingTaskTypes)
    - `filingType.get|list` (viewFilingTypes)
  - Matters
    - `matterView.get|list` (viewMatters)
    - `patentMatterView.get|list` (viewPatentMatters)
    - `trademarkMatterView.get|list` (viewTrademarkMatters)
  - Responses
    - `responseView.get|list` (viewResponses)
    - `responsesIncompleteView.get|list` (viewResponsesIncomplete)
    - `responseTaskTypeView.get|list` (viewResponseTaskTypes)
  - Dashboards
    - `filingsDashboardEntry.get|list` (filingsDashboardComplete) — list supports `clientEntityId?`, `withParalegal?`, `needsExaminerInterview?`, `limit?`, `offset?`
    - `responsesDashboardEntry.get|list` (responsesDashboardComplete) — list supports `clientEntityId?`, `withParalegal?`, `limit?`, `offset?`
  - Tasks
    - `taskView.get|list` (viewTasks)
    - `taskClass.list` (viewTaskClass) — result: `{ items: [{ id, taskClassName }] }`
    - `taskTypeView.get|list` (viewTaskTypes)
  - Flags/Tags
    - `objectFlagView.get` `{ id }` or `{ objectId, camelCase }`
    - `objectFlagView.listForObject` `{ objectId }` → `{ objectId, items }`
    - `objectTagView.get` `{ id }` or `{ objectId, tagId? | tagText? }`
    - `objectTagView.listForObject` `{ objectId }` → `{ objectId, items }`
  - Appointments
    - `upcomingAppointmentView.get|list` (viewUpcomingAppointments)

Notes
- All view `.get` actions try common id columns (e.g., `Id`, `…Id`) to locate the row and return `ENOTFOUND` if not present.
- Dates in views are encoded as ISO strings (YYYY‑MM‑DD or ISO‑8601).

Additional Table Endpoints (summary)
- `deadline.list|create|delete`
- `flagClass.list|create|delete`
- `object.list|create|delete`
- `tag.list|create|update|delete`
- `task.list`
- `taskType.list|create|update|delete`
