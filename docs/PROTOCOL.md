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
