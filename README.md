# seancrandall
# Nutmeg

Nutmeg is a pre-alpha project. It's a docket manager for patent attorneys. I'm mostly making it for myself because I don't like the options that are available. 

Built using Qt 6.8. Compile with Clang or Visual C++. Doesn't work with gcc because I use __declspec-style properties for convenience.

## nmbroker (backend) daemon

Run the WebSocket backend as a standalone service:

```
cd nmbroker && qmake QMAKE_CXX=clang++ QMAKE_CC=clang && make -j
./nmbroker --port 8787 --bind 0.0.0.0 --detach --pidfile /tmp/nmbroker.pid --log-file /var/log/nmbroker.log
```

Options:
- `-p, --port <port>`: Listen port (overrides `NUTMEG_WS_PORT` and settings).
- `-b, --bind <addr>`: Bind address (IPv4/IPv6 literal or `localhost`).
- `-d, --detach`: Run in background (start detached process).
- `--pidfile <file>`: Write process ID after successful start.
- `--log-file <file>`: Append logs to file (UTC timestamps).
- `--config <file>`: Use specific INI config file for settings.
- `--db-host <host>`: Database host.
- `--db-port <port>`: Database port.
- `--db-user <user>`: Database username.
- `--db-password <password>`: Database password.
- `--db-name <name>`: Database/schema name.

Settings keys (INI via QSettings):
- `websocket/port` (default `8787`)
- `websocket/bind` (default `0.0.0.0`)

Environment:
- `NUTMEG_WS_PORT` sets the port (overrides settings).
- `LOG` sets the log file path if `--log-file` is not provided.
- `NUTMEG_DB_HOST`, `NUTMEG_DB_PORT`, `NUTMEG_DB_USER`, `NUTMEG_DB_PASSWORD` override DB settings.
- `NUTMEG_DB_NAME` overrides the database/schema name.

## Test client (Python/Qt)

A simple GUI test client is included at `nmbroker/testbroker.py` (PyQt6 required):

```
python3 nmbroker/testbroker.py
```

- Connect to a broker, then use the Objects tab to `get`/`update` objects by id.
- The Actions tab lists endpoints parsed from `docs/PROTOCOL.md`; select one, edit payload JSON, and send.

Requirements:
- Python 3.8 or newer.
- One of the Qt Python bindings with WebSockets support:
  - `pip install PyQt6`
  - or `pip install PySide6`
  The script imports `PyQt6.QtWebSockets`. If you prefer PySide6, adjust imports accordingly.
