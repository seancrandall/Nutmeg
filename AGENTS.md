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
- Compiler: Clang or MSVC. GCC is not supported (code uses `__declspec`-style properties).
- QMake-based builds.
  - Example (Unix-like): `cd nmgui && qmake && make` or `cd nmbroker && qmake && make`.
  - Example (Windows MSVC): Open in Qt Creator with MSVC kit or run `qmake && nmake` in a VS-enabled shell.

Current Direction
- Up to now, `nmgui` and `nmbroker` have been essentially integrated.
- Goal: First solidify the backend (`nmbroker`) so it serves data over a WebSocket API.
- Rationale: Decoupling enables a web-based frontend while preserving the desktop GUI.

WebSocket Server
- Module: lives in `nmbroker` using Qt's `QtWebSockets`.
- Default port: `8787`.
- Config order: environment `NUTMEG_WS_PORT` → `QSettings` key `websocket/port` → default.
- Docker: pass `-e NUTMEG_WS_PORT=9010` and map `-p 9010:9010` (or desired port).
- Behavior: replies `pong` to `ping`; echoes text in a JSON envelope while protocol evolves.

Protocol
- Transport: WebSocket, JSON over UTF‑8 text.
- Envelope: `type=req|res|event`, `id`, `action`, `payload`, `ok/result|error`.
- Version: include `version` in requests; server announces via `server.hello` event.
- See `docs/PROTOCOL.md` for the detailed schema, examples, and error codes.

Backend WebSocket Plan (initial guidance)
- Add a WebSocket server inside `nmbroker` (Qt 6 provides `QtWebSockets`).
- Serve application data and operations via messages (e.g., JSON payloads).
- Keep domain/model code in `nmbroker` and expose it through the socket layer (no direct DB logic in the UI).
- Maintain compatibility with the existing GUI during the transition (feature flags or separate entry points).

Working Guidelines
- Align with existing code style and structure; keep changes minimal and focused.
- Prefer adding new backend interfaces in `nmbroker/*` and only thin adapters in `nmgui/*`.
- Do not introduce GCC-only constructs; verify Clang/MSVC compatibility.
- Treat `*.pro.user`, generated Makefiles, and `nmgui/build/*` as non-source artifacts (do not hand-edit).
- When creating new services/endpoints, document message schemas and connection lifecycle.

Notes for Agents
- If you add or modify files, keep this AGENTS.md updated with any conventions or architectural decisions.
- See `README.md` for a brief project overview.
