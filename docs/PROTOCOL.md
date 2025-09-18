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

Compatibility with current server
- Current implementation sends a greeting and supports text `ping` → `pong` and echoing messages.
- It will be updated to use the `type=req/res/event` envelope; existing text `ping` will continue to work during transition.
