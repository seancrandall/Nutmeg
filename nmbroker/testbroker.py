#!/usr/bin/env python3
# Minimal Qt-based test client for nmbroker WebSocket API
# Requires: PyQt6 (with QtWebSockets)

import os
import re
import sys
import json
from pathlib import Path

from PyQt6.QtCore import Qt, QUrl, QObject, pyqtSignal, QTimer
from PyQt6.QtWidgets import (
    QApplication, QWidget, QMainWindow, QVBoxLayout, QHBoxLayout, QGridLayout,
    QLabel, QLineEdit, QPushButton, QTextEdit, QListWidget, QListWidgetItem,
    QTabWidget, QComboBox, QSplitter, QSizePolicy
)
from PyQt6.QtWebSockets import QWebSocket


API_VERSION = "0.1"


def load_actions_from_protocol_md(repo_root: Path) -> list[str]:
    """Parse docs/PROTOCOL.md for backticked action names.
    Expands patterns like `foo.get|list|create` to [foo.get, foo.list, foo.create].
    """
    actions: set[str] = set()
    protocol_md = repo_root / "docs" / "PROTOCOL.md"
    if not protocol_md.exists():
        return []
    text = protocol_md.read_text(encoding="utf-8", errors="ignore")
    # Find all backtick content
    for token in re.findall(r"`([^`]+)`", text):
        token = token.strip()
        # Only keep tokens that look like actions (contain a dot, no spaces/braces/quotes)
        if "." not in token:
            continue
        if any(ch in token for ch in " {}[]:\"\'\t\n"):
            continue
        # Expand foo.get|list
        if "|" in token:
            # split on first dot
            if "." in token:
                base, rest = token.split(".", 1)
                parts = [p for p in rest.split("|") if p]
                for p in parts:
                    actions.add(f"{base}.{p}")
            else:
                continue
        else:
            actions.add(token)
    # De-duplicate and sort
    return sorted(actions)


def discover_object_types(objects_dir: Path) -> list[str]:
    """Return a list of core object resource names from nmbroker/objects.
    Filters out dashboard/flag/tag-only items. LowerCamel based on filenames.
    """
    keep: set[str] = set()
    if not objects_dir.exists():
        return []
    # Map headers in objects/ to resource names
    for p in objects_dir.glob("*.h"):
        name = p.stem  # e.g., matter, patentmatter
        # Skip non-entity helpers
        if name in {"flag", "flagclass", "tag", "filingsdashboardentry", "responsesdashboardentry"}:
            continue
        # Keep
        # Convert to lowerCamel if needed (headers are already lower)
        keep.add(name)
    # Favor known set from protocol if present
    preferred_order = [
        "appointment", "deadline", "document", "enterprise", "entity", "filing",
        "matter", "object", "person", "response", "task",
        "trademarkmatter", "patentmatter", "copyrightmatter",
    ]
    # normalize discovered names to lower
    disc = {n.lower() for n in keep}
    ordered = [n for n in preferred_order if n in disc]
    # append any others not in preferred list
    for n in sorted(disc):
        if n not in ordered:
            ordered.append(n)
    return ordered


class BrokerClient(QObject):
    connectedChanged = pyqtSignal(bool)
    messageReceived = pyqtSignal(dict)
    textReceived = pyqtSignal(str)

    def __init__(self, parent=None):
        super().__init__(parent)
        self._ws = QWebSocket()
        self._ws.connected.connect(lambda: self.connectedChanged.emit(True))
        self._ws.disconnected.connect(lambda: self.connectedChanged.emit(False))
        self._ws.textMessageReceived.connect(self._on_text)
        self._next_id = 1

    def open(self, host: str, port: int):
        url = QUrl(f"ws://{host}:{port}")
        self._ws.open(url)

    def close(self):
        self._ws.close()

    def is_connected(self) -> bool:
        return self._ws.isValid()

    def send(self, action: str, payload: dict):
        rid = str(self._next_id)
        self._next_id += 1
        req = {
            "type": "req",
            "id": rid,
            "action": action,
            "payload": payload or {},
            "version": API_VERSION,
        }
        self._ws.sendTextMessage(json.dumps(req))
        return rid

    def _on_text(self, txt: str):
        # Try JSON first, then emit raw
        try:
            obj = json.loads(txt)
            self.messageReceived.emit(obj)
        except Exception:
            self.textReceived.emit(txt)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Nutmeg Broker Tester")
        self.resize(1100, 750)

        repo_root = Path(__file__).resolve().parent.parent
        self.actions = load_actions_from_protocol_md(repo_root)
        self.object_types = discover_object_types(repo_root / "nmbroker" / "objects")

        self.client = BrokerClient(self)
        self.client.connectedChanged.connect(self._on_connected_changed)
        self.client.messageReceived.connect(self._on_message)
        self.client.textReceived.connect(self._append_text)

        # Track pending requests we care about (by request id)
        # value: 'obj_get' | 'obj_upd'
        self._pending: dict[str, str] = {}

        self._build_ui()

    def _build_ui(self):
        host_lbl = QLabel("Host:")
        self.host_edit = QLineEdit("localhost")
        port_lbl = QLabel("Port:")
        self.port_edit = QLineEdit("8787")
        self.port_edit.setMaximumWidth(90)
        self.btn_connect = QPushButton("Connect")
        self.btn_disconnect = QPushButton("Disconnect")
        self.status_lbl = QLabel("Disconnected")

        conn_row = QHBoxLayout()
        conn_row.addWidget(host_lbl)
        conn_row.addWidget(self.host_edit)
        conn_row.addWidget(port_lbl)
        conn_row.addWidget(self.port_edit)
        conn_row.addWidget(self.btn_connect)
        conn_row.addWidget(self.btn_disconnect)
        conn_row.addStretch(1)
        conn_row.addWidget(self.status_lbl)

        self.btn_connect.clicked.connect(self._connect)
        self.btn_disconnect.clicked.connect(self._disconnect)
        self.port_edit.returnPressed.connect(self._connect)

        tabs = QTabWidget()
        tabs.addTab(self._build_objects_tab(), "Objects")
        tabs.addTab(self._build_actions_tab(), "Actions")

        self.output = QTextEdit()
        self.output.setReadOnly(True)
        self.output.setLineWrapMode(QTextEdit.LineWrapMode.NoWrap)

        # Last action status line
        status_row = QHBoxLayout()
        status_row.addWidget(QLabel("Last Action:"))
        self.action_status_lbl = QLabel("")
        self.action_status_lbl.setTextInteractionFlags(Qt.TextInteractionFlag.TextSelectableByMouse)
        status_row.addWidget(self.action_status_lbl)
        status_row.addStretch(1)

        top = QWidget()
        top_v = QVBoxLayout(top)
        top_v.addLayout(conn_row)
        top_v.addWidget(tabs)
        top_v.addLayout(status_row)
        top_v.addWidget(QLabel("Responses / Log:"))
        top_v.addWidget(self.output)

        self.setCentralWidget(top)
        # Auto-connect on startup (after UI is ready)
        QTimer.singleShot(0, self._connect)

    def _build_objects_tab(self) -> QWidget:
        w = QWidget()
        lay = QGridLayout(w)

        lay.addWidget(QLabel("Object:"), 0, 0)
        self.object_combo = QComboBox()
        self.object_combo.addItems(self.object_types)
        lay.addWidget(self.object_combo, 0, 1)

        lay.addWidget(QLabel("ID:"), 0, 2)
        self.object_id = QLineEdit()
        self.object_id.setPlaceholderText("numeric id")
        self.object_id.setMaximumWidth(160)
        lay.addWidget(self.object_id, 0, 3)

        self.btn_obj_get = QPushButton("Get")
        self.btn_obj_upd = QPushButton("Update")
        lay.addWidget(self.btn_obj_get, 0, 4)
        lay.addWidget(self.btn_obj_upd, 0, 5)

        self.obj_json = QTextEdit()
        self.obj_json.setPlaceholderText("Object JSON will appear here. Edit then click Update to send <object>.update.")
        self.obj_json.setLineWrapMode(QTextEdit.LineWrapMode.NoWrap)
        lay.addWidget(self.obj_json, 1, 0, 1, 6)

        self.btn_obj_get.clicked.connect(self._on_obj_get)
        self.btn_obj_upd.clicked.connect(self._on_obj_upd)

        return w

    def _build_actions_tab(self) -> QWidget:
        w = QWidget()
        h = QHBoxLayout(w)

        # Left: actions list
        left = QVBoxLayout()
        left.addWidget(QLabel("Actions (from PROTOCOL.md):"))
        self.actions_list = QListWidget()
        for a in self.actions:
            QListWidgetItem(a, self.actions_list)
        self.actions_list.setSelectionMode(self.actions_list.SelectionMode.SingleSelection)
        left.addWidget(self.actions_list)

        # Right: payload editor and send
        right = QVBoxLayout()
        right.addWidget(QLabel("Payload (JSON):"))
        self.payload_edit = QTextEdit()
        self.payload_edit.setPlaceholderText("{ }")
        self.payload_edit.setLineWrapMode(QTextEdit.LineWrapMode.NoWrap)
        self.payload_edit.setPlainText("{}")
        right.addWidget(self.payload_edit)
        self.btn_send = QPushButton("Send Selected Action")
        right.addWidget(self.btn_send)
        right.addStretch(1)

        self.actions_list.currentItemChanged.connect(self._on_action_selected)
        self.btn_send.clicked.connect(self._on_send_action)

        h.addLayout(left, 2)
        h.addLayout(right, 3)
        return w

    # ---- UI callbacks ----
    def _connect(self):
        host = self.host_edit.text().strip() or "localhost"
        try:
            port = int(self.port_edit.text().strip() or "8787")
        except ValueError:
            self._append_text("Invalid port")
            return
        self.client.open(host, port)

    def _disconnect(self):
        self.client.close()

    def _on_connected_changed(self, ok: bool):
        self.status_lbl.setText("Connected" if ok else "Disconnected")

    def _append_json(self, obj: dict):
        try:
            pretty = json.dumps(obj, indent=2, sort_keys=True)
        except Exception:
            pretty = str(obj)
        self.output.append(pretty)

    def _append_text(self, txt: str):
        self.output.append(txt)

    def _on_obj_get(self):
        res = self.object_combo.currentText().strip()
        id_txt = self.object_id.text().strip()
        try:
            rid = int(id_txt)
        except ValueError:
            self._append_text("Enter a numeric ID")
            return
        action = f"{res}.get"
        # Clear previous object JSON to prepare a clean view
        self.obj_json.clear()
        req_id = self.client.send(action, {"id": rid})
        self._pending[req_id] = 'obj_get'

    def _on_obj_upd(self):
        res = self.object_combo.currentText().strip()
        action = f"{res}.update"
        txt = self.obj_json.toPlainText().strip()
        if not txt:
            self._append_text("No JSON to send")
            return
        try:
            payload = json.loads(txt)
            if not isinstance(payload, dict):
                raise ValueError("payload must be JSON object")
        except Exception as e:
            self._append_text(f"Invalid JSON: {e}")
            return
        req_id = self.client.send(action, payload)
        self._pending[req_id] = 'obj_upd'

    def _on_action_selected(self, cur: QListWidgetItem, prev: QListWidgetItem):
        if not cur:
            return
        name = cur.text()
        # Provide a tiny payload template for common patterns
        template = {}
        if name.endswith(".get"):
            template = {"id": 1}
        elif name.endswith(".listForObject"):
            template = {"objectId": 1}
        elif name.endswith(".delete"):
            template = {"id": 1}
        elif name.endswith(".create"):
            template = {"<field>": "<value>"}
        self.payload_edit.setPlainText(json.dumps(template, indent=2))

    def _on_send_action(self):
        cur = self.actions_list.currentItem()
        if not cur:
            self._append_text("Select an action")
            return
        name = cur.text()
        txt = self.payload_edit.toPlainText().strip() or "{}"
        try:
            payload = json.loads(txt)
            if not isinstance(payload, dict):
                raise ValueError("payload must be JSON object")
        except Exception as e:
            self._append_text(f"Invalid JSON payload: {e}")
            return
        self.client.send(name, payload)

    # ---- WS callbacks ----
    def _on_message(self, obj: dict):
        # Always append to the log
        self._append_json(obj)
        try:
            if not isinstance(obj, dict):
                return
            if obj.get('type') != 'res':
                return
            rid = str(obj.get('id', ''))
            if not rid:
                return
            ctx = self._pending.pop(rid, None)
            if not ctx:
                return
            ok = bool(obj.get('ok'))
            if ok:
                result_obj = obj.get('result', {})
                pretty = json.dumps(result_obj, indent=2, sort_keys=True)
            else:
                result_obj = obj.get('error', obj)
                pretty = json.dumps(result_obj, indent=2, sort_keys=True)
            if ctx in ('obj_get', 'obj_upd'):
                # Replace the editor content with the latest server view
                self.obj_json.setPlainText(pretty)
                # Auto-fill ID field if present in result
                if isinstance(result_obj, dict) and 'id' in result_obj:
                    try:
                        rid_val = result_obj.get('id')
                        # Cast floats that are integral to int
                        if isinstance(rid_val, float) and rid_val.is_integer():
                            rid_text = str(int(rid_val))
                        else:
                            rid_text = str(rid_val)
                        self.object_id.setText(rid_text)
                    except Exception:
                        pass
                # Update action status line
                from datetime import datetime
                when = datetime.now().isoformat(sep=' ', timespec='seconds')
                act = 'GET' if ctx == 'obj_get' else 'UPDATE'
                status = f"{when} — {act} {'ok' if ok else 'error'}"
                self.action_status_lbl.setText(status)
        except Exception:
            # Keep UI resilient even if parsing fails
            pass


def main():
    app = QApplication(sys.argv)
    mw = MainWindow()
    mw.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
