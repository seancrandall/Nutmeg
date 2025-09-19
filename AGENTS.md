# Nutmeg — Agent Guide (AGENTS.md)

Purpose
- Manage an attorney docket with a custom C++/Qt application.
- Two components: `nmbroker` (backend) and `nmgui` (frontend GUI).

Repository Structure
- `nmbroker/`: Core data layer and app logic (backend). QMake project in `nmbroker/nmbroker.pro`.
- `nmgui/`: Qt Widgets GUI (frontend). QMake project in `nmgui/nmgui.pro`.
- Resources/translations: `*.qrc`, `*_en_US.ts` live alongside sources.
- Note: A `build/` tree and `*.pro.user` files exist from Qt Creator; treat as local artifacts.

Build Notes
- Qt: Target Qt 6.8.
- Compiler: Always use Clang (or MSVC on Windows). GCC is not supported because the code relies on `__declspec`-style properties. We also require `-fdeclspec` when using Clang on Unix-like systems.
- QMake-based builds.
  - Backend (`nmbroker`) build (Unix-like, Clang): `cd nmbroker && qmake QMAKE_CXX=clang++ QMAKE_CC=clang && make -j`
  - Windows MSVC: Open in Qt Creator with the MSVC kit or run `qmake && nmake` in a Developer Command Prompt.
  - Example (Windows MSVC): Open in Qt Creator with MSVC kit or run `qmake && nmake` in a VS-enabled shell.
  - Ensure the Qt WebSockets module is installed (`QtWebSockets`). On Linux distros, install the `qt6-websockets` dev package.

Current Direction
- Up to now, `nmgui` and `nmbroker` have been essentially integrated.
- Goal: First solidify the backend (`nmbroker`) so it serves data over a WebSocket API.
- Rationale: Decoupling enables a web-based frontend while preserving the desktop GUI.

WebSocket Server
- Module: lives in `nmbroker` using Qt's `QtWebSockets`.
- Default port: `8787`.
- Config precedence:
  - Port: CLI `--port` → env `NUTMEG_WS_PORT` → `QSettings` `websocket/port` → default `8787`.
  - Bind: CLI `--bind` → `QSettings` `websocket/bind` → default `0.0.0.0`.
- Docker: pass `-e NUTMEG_WS_PORT=9010` and map `-p 9010:9010` (or desired port).
- Startup event: emits `server.hello { service, apiVersion }`.
- Protocol: request/response envelope with `type=req|res|event`. See `docs/PROTOCOL.md`.

Backend Status (WS endpoints)
- Core: `info.get`, `ping`.
- Objects (get/update where it makes sense): `appointment`, `copyrightMatter`, `deadline`, `document`, `enterprise`, `entity`, `filing`, `matter`, `object`, `person`, `response`, `task`, `trademarkMatter`, `patentMatter`.
- Flags/Tags: `flag.get`, `flag.set`, `tag.get`, `tag.search`, `object.tag`.
- Taxonomies: `taskClass.get`, `taskType.get`.
- Dashboards (read-only): `filingsDashboardEntry.get`, `responsesDashboardEntry.get`.
- Details, payloads, and examples are in `docs/PROTOCOL.md` (also includes a small JS client snippet).

Recent WebSocket Additions (Batch Models)
- Views are read-only; Tables are read/write. We added WS coverage across `dbaccess/` models.
- Views (all return full view columns)
  - Added: `client.get/list`, `contractingFirm.get/list`, `entityView.get/list`, `peopleView.get/list`, `paralegalView.get/list`, `patentExaminerView.get/list`, `workAttorneyView.get/list`.
  - Filings: `filingView.get/list`, `filingIncompleteView.get/list`, `filingTaskType.get/list`, `filingType.get/list`, `patentFilingTypeView.get/list`, `trademarkFilingTypeView.get/list`.
  - Matters: `matterView.get/list`, `patentMatterView.get/list`, `trademarkMatterView.get/list`.
  - Responses: `responseView.get/list`, `responsesIncompleteView.get/list`, `responseTaskTypeView.get/list`.
  - Tasks: `taskView.get/list`, `taskClass.list`, `taskTypeView.get/list`.
  - Flags/Tags: `objectFlagView.get`, `objectFlagView.listForObject`, `objectTagView.get`, `objectTagView.listForObject`.
  - Appointments: `appointmentObject.get`, `appointmentObject.listForObject`, `upcomingAppointmentView.get/list`.
- Tables
  - `appointmentType.get/list/create/update/delete`.
  - `deadline.get/update` (existing) plus `deadline.list/create/delete`.
  - `flagClass.get/update` (existing) plus `flagClass.list/create/delete`.
  - `object.get/update` (existing) plus `object.list/create/delete`.
  - `tag.get/search` (existing) plus `tag.list/create/update/delete`.
  - `task.get/update` (existing) plus `task.list`.
  - `taskType.get` (existing) plus `taskType.list/create/update/delete`.
  - `objectType.get/list/update` plus `objectType.create/delete`.
- Dashboards (read-only)
  - `responsesDashboardEntry.get/list` and `filingsDashboardEntry.get/list` (list supports simple filters and paging; see docs).

Action Naming Conventions
- Tables: `resource.get|list|create|update|delete`.
- Views: `resourceView.get|list` or domain-specific names (`client.list`, `appointmentObject.get`).
- Use ISO-8601 strings for dates; server returns dates as `YYYY-MM-DD` or full ISO where appropriate.
- Error codes: `EBADREQ`, `ENOTFOUND`, `EINSERT`, `EUPDATE`, `EDELETE`.

Where to Add Endpoints
- Register actions in `nmbroker/websocketserver.cpp` using the existing `m_router.registerAction` pattern.
- For tables, prefer domain setters; otherwise use `Nutdb::UpdateField` or direct SQL when no domain object exists.
- Keep payload validation minimal and consistent with existing handlers; return structured errors.

Testing Tips
- Build: `cd nmbroker && qmake QMAKE_CXX=clang++ QMAKE_CC=clang && make -j`.
- Quick check with `wscat` or a simple JS snippet (see `docs/PROTOCOL.md` for example).
- First message on connect: `server.hello { service, apiVersion }`.

Daemon CLI
- `-p, --port <port>`: Listen port.
- `-b, --bind <addr>`: Bind address (IPv4/IPv6 or `localhost`).
- `-d, --detach`: Run in background (re-exec detached).
- `--pidfile <file>`: Write PID to file after start.
- `--log-file <file>`: Append logs to this file. If not set, `LOG` env var can be used.
- `--config <file>`: Use an explicit INI config file for `QSettings`.

Settings keys
- `websocket/port` (uint, default `8787`)
- `websocket/bind` (string, default `0.0.0.0`)

Project File notes (nmbroker.pro)
- Clang-only flags are assumed; translations build steps are disabled by default to avoid `lrelease` requirement.
- Db/model sources are curated to satisfy links for Nutdb-backed calls. GUI-dependent objects (e.g., some email utils) are excluded from the backend build.

Protocol
- Transport: WebSocket, JSON over UTF‑8 text.
- Envelope: `type=req|res|event`, `id`, `action`, `payload`, `ok/result|error`.
- Version: include `version` in requests; server announces via `server.hello` event.
- See `docs/PROTOCOL.md` for the detailed schema, examples, and error codes.

Backend WebSocket Plan (initial guidance)
- Keep domain/model code in `nmbroker` and expose it through the socket layer (no direct DB logic in the UI).
- Maintain compatibility with the existing GUI during the transition (feature flags or separate entry points).
- To add endpoints, extend `nmbroker/websocketserver.cpp` (Router registrations) and document in `docs/PROTOCOL.md`.

Dev Tips
- Router lives in `websocketserver.cpp` via a simple schema + handler registry (see `WSRouter`).
- Validate payloads and return structured errors: `EBADREQ`, `ENOTFOUND`, etc.
- Prefer using existing setters on domain objects; when unavailable or protected, use `Nutdb::UpdateField` as a thin persistence path.
- After implementing endpoints, rebuild with Clang and update `docs/PROTOCOL.md` accordingly.

Database Config
- CLI: `--db-host`, `--db-port`, `--db-user`, `--db-password`, `--db-name` override environment and settings.
- Env: `NUTMEG_DB_HOST`, `NUTMEG_DB_PORT`, `NUTMEG_DB_USER`, `NUTMEG_DB_PASSWORD`, `NUTMEG_DB_NAME` override settings.
- Settings: falls back to `database/server`, `database/port`, `database/username`, `database/password`, `database/database_name`.

Working Guidelines
- Align with existing code style and structure; keep changes minimal and focused.
- Prefer adding new backend interfaces in `nmbroker/*` and only thin adapters in `nmgui/*`.
- Do not introduce GCC-only constructs; verify Clang/MSVC compatibility.
- Treat `*.pro.user`, generated Makefiles, and `nmgui/build/*` as non-source artifacts (do not hand-edit).
- When creating new services/endpoints, document message schemas and connection lifecycle.

Notes for Agents
- If you add or modify files, keep this AGENTS.md updated with any conventions or architectural decisions.
- See `README.md` for a brief project overview.
