#include "websocketserver.h"

#include <QWebSocketServer>
#include <QWebSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>
#include <QDebug>
#include "objects/matter.h"

namespace Nutmeg {

WebSocketServer::WebSocketServer(quint16 port, QObject *parent)
    : QObject(parent)
    , m_server(new QWebSocketServer(QStringLiteral("NutmegBroker"),
                                    QWebSocketServer::NonSecureMode,
                                    this))
{
    const bool ok = m_server->listen(QHostAddress::Any, port);
    if (!ok) {
        qWarning() << "WebSocketServer failed to listen on port" << port
                   << ":" << m_server->errorString();
    } else {
        qInfo() << "WebSocketServer listening on" << m_server->serverAddress()
                << ":" << m_server->serverPort();
    }

    connect(m_server, &QWebSocketServer::newConnection,
            this, &WebSocketServer::onNewConnection);

    // Register built-in actions on the router
    m_router.registerAction(QStringLiteral("ping"), ActionSpec{
        /*fields*/ {},
        /*handler*/ [](const QJsonObject &){
            DispatchResult r; r.ok = true;
            r.result = QJsonObject{
                {"pong", true},
                {"now", QDateTime::currentDateTimeUtc().toString(Qt::ISODate)}
            };
            return r;
        }
    });

    m_router.registerAction(QStringLiteral("info.get"), ActionSpec{
        /*fields*/ {},
        /*handler*/ [this](const QJsonObject &){
            DispatchResult r; r.ok = true;
            r.result = QJsonObject{
                {"service", QStringLiteral("NutmegBroker")},
                {"apiVersion", protocolVersion()}
            };
            return r;
        }
    });

    // matter.get: by id
    m_router.registerAction(QStringLiteral("matter.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const MatterData m = Nutdb::GetMatter(id);
            DispatchResult r;
            if (m.MatterId == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Matter not found");
                return r;
            }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.MatterId)},
                {"fkParent", static_cast<double>(m.fkParent)},
                {"attorneyDocketNumber", m.AttorneyDocketNumber},
                {"clientDocketNumber", m.ClientDocketNumber},
                {"title", m.Title},
                {"fkClient", static_cast<double>(m.fkClient)},
                {"fkAssigningFirm", static_cast<double>(m.fkAssigningFirm)},
                {"fkDefaultWorkAttorney", static_cast<double>(m.fkDefaultWorkAttorney)},
                {"fkDefaultParalegal", static_cast<double>(m.fkDefaultParalegal)},
                {"fkKeyDocument", static_cast<double>(m.fkKeyDocument)},
                {"fkMatterJurisdiction", static_cast<double>(m.fkMatterJurisdiction)},
                {"oldMatterId", static_cast<double>(m.OldMatterId)}
            };
            return r;
        }
    });

    // matter.update: updates provided fields; returns updated record
    m_router.registerAction(QStringLiteral("matter.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("fkParent"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("attorneyDocketNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("clientDocketNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("title"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkClient"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkAssigningFirm"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkDefaultWorkAttorney"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkDefaultParalegal"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkKeyDocument"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkMatterJurisdiction"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("oldMatterId"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Matter matter{id};
            DispatchResult r;
            if (matter.getId() == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Matter not found");
                return r;
            }

            // Apply provided fields with explicit type checks
            if (payload.contains("fkParent") && payload.value("fkParent").isDouble())
                matter.SetfkParent(static_cast<Key>(payload.value("fkParent").toDouble()));
            if (payload.contains("attorneyDocketNumber") && payload.value("attorneyDocketNumber").isString())
                matter.SetAttorneyDocketNumber(payload.value("attorneyDocketNumber").toString());
            if (payload.contains("clientDocketNumber") && payload.value("clientDocketNumber").isString())
                matter.SetClientDocketNumber(payload.value("clientDocketNumber").toString());
            if (payload.contains("title") && payload.value("title").isString())
                matter.SetTitle(payload.value("title").toString());
            if (payload.contains("fkClient") && payload.value("fkClient").isDouble())
                matter.SetfkClient(static_cast<Key>(payload.value("fkClient").toDouble()));
            if (payload.contains("fkAssigningFirm") && payload.value("fkAssigningFirm").isDouble())
                matter.SetfkAssigningFirm(static_cast<Key>(payload.value("fkAssigningFirm").toDouble()));
            if (payload.contains("fkDefaultWorkAttorney") && payload.value("fkDefaultWorkAttorney").isDouble())
                matter.SetfkDefaultWorkAttorney(static_cast<Key>(payload.value("fkDefaultWorkAttorney").toDouble()));
            if (payload.contains("fkDefaultParalegal") && payload.value("fkDefaultParalegal").isDouble())
                matter.SetfkDefaultParalegal(static_cast<Key>(payload.value("fkDefaultParalegal").toDouble()));
            if (payload.contains("fkKeyDocument") && payload.value("fkKeyDocument").isDouble())
                matter.SetfkKeyDocument(static_cast<Key>(payload.value("fkKeyDocument").toDouble()));
            if (payload.contains("fkMatterJurisdiction") && payload.value("fkMatterJurisdiction").isDouble())
                matter.SetfkMatterJurisdiction(static_cast<Key>(payload.value("fkMatterJurisdiction").toDouble()));
            if (payload.contains("oldMatterId") && payload.value("oldMatterId").isDouble())
                matter.SetOldId(static_cast<Key>(payload.value("oldMatterId").toDouble()));

            // Return fresh record
            const MatterData m = Nutdb::GetMatter(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.MatterId)},
                {"fkParent", static_cast<double>(m.fkParent)},
                {"attorneyDocketNumber", m.AttorneyDocketNumber},
                {"clientDocketNumber", m.ClientDocketNumber},
                {"title", m.Title},
                {"fkClient", static_cast<double>(m.fkClient)},
                {"fkAssigningFirm", static_cast<double>(m.fkAssigningFirm)},
                {"fkDefaultWorkAttorney", static_cast<double>(m.fkDefaultWorkAttorney)},
                {"fkDefaultParalegal", static_cast<double>(m.fkDefaultParalegal)},
                {"fkKeyDocument", static_cast<double>(m.fkKeyDocument)},
                {"fkMatterJurisdiction", static_cast<double>(m.fkMatterJurisdiction)},
                {"oldMatterId", static_cast<double>(m.OldMatterId)}
            };
            return r;
        }
    });
}

WebSocketServer::~WebSocketServer()
{
    for (QWebSocket *sock : m_clients) {
        sock->close();
        sock->deleteLater();
    }
}

bool WebSocketServer::isListening() const
{
    return m_server->isListening();
}

quint16 WebSocketServer::port() const
{
    return static_cast<quint16>(m_server->serverPort());
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();
    if (!socket)
        return;

    m_clients << socket;

    connect(socket, &QWebSocket::textMessageReceived,
            this, &WebSocketServer::processTextMessage);
    connect(socket, &QWebSocket::disconnected,
            this, &WebSocketServer::socketDisconnected);

    emit clientConnected(socket->peerAddress().toString());
    qInfo() << "WebSocket client connected:" << socket->peerAddress().toString();

    // Protocol v0: send server.hello event
    QJsonObject helloPayload{
        {"service", QStringLiteral("NutmegBroker")},
        {"apiVersion", protocolVersion()}
    };
    sendEvent(socket, QStringLiteral("server.hello"), helloPayload);
}

void WebSocketServer::processTextMessage(const QString &message)
{
    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    if (!socket)
        return;

    const QString trimmed = message.trimmed();
    if (trimmed.compare(QStringLiteral("ping"), Qt::CaseInsensitive) == 0) {
        socket->sendTextMessage(QStringLiteral("pong"));
        return;
    }

    // Expect JSON envelope
    QJsonParseError perr{};
    const QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &perr);
    if (perr.error != QJsonParseError::NoError || !doc.isObject()) {
        QJsonObject req; // empty
        sendResponse(socket, makeErr(req, QStringLiteral("EBADREQ"), QStringLiteral("Invalid JSON")));
        return;
    }
    const QJsonObject req = doc.object();
    const QString type = req.value(QStringLiteral("type")).toString();
    if (!(type.compare(QStringLiteral("req"), Qt::CaseInsensitive) == 0)) {
        sendResponse(socket, makeErr(req, QStringLiteral("EBADREQ"), QStringLiteral("Expected type 'req'")));
        return;
    }

    handleAction(socket, req);
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    if (!socket)
        return;

    m_clients.removeAll(socket);
    emit clientDisconnected();
    qInfo() << "WebSocket client disconnected";
    socket->deleteLater();
}

} // namespace Nutmeg

// ===== Protocol helpers & dispatch =====

namespace Nutmeg {

void WebSocketServer::sendEvent(QWebSocket *socket, const QString &event, const QJsonObject &payload)
{
    QJsonObject evt{
        {"type", QStringLiteral("event")},
        {"event", event},
        {"payload", payload}
    };
    socket->sendTextMessage(QString::fromUtf8(QJsonDocument(evt).toJson(QJsonDocument::Compact)));
}

void WebSocketServer::sendResponse(QWebSocket *socket, const QJsonObject &res)
{
    socket->sendTextMessage(QString::fromUtf8(QJsonDocument(res).toJson(QJsonDocument::Compact)));
}

QJsonObject WebSocketServer::makeOk(const QJsonObject &req, const QJsonObject &result)
{
    QJsonObject res{
        {"type", QStringLiteral("res")},
        {"ok", true},
        {"result", result}
    };
    if (req.contains("id")) res.insert("id", req.value("id"));
    return res;
}

QJsonObject WebSocketServer::makeErr(const QJsonObject &req, const QString &code, const QString &message, const QJsonObject &details)
{
    QJsonObject err{
        {"code", code},
        {"message", message}
    };
    if (!details.isEmpty()) err.insert("details", details);

    QJsonObject res{
        {"type", QStringLiteral("res")},
        {"ok", false},
        {"error", err}
    };
    if (req.contains("id")) res.insert("id", req.value("id"));
    return res;
}

void WebSocketServer::handleAction(QWebSocket *socket, const QJsonObject &req)
{
    const QString action = req.value(QStringLiteral("action")).toString();
    const QJsonObject payload = req.value(QStringLiteral("payload")).toObject();

    if (action.isEmpty()) {
        sendResponse(socket, makeErr(req, QStringLiteral("EBADREQ"), QStringLiteral("Missing action")));
        return;
    }

    // Dispatch via router with validation
    const DispatchResult r = m_router.dispatch(action, payload);
    if (r.ok) {
        sendResponse(socket, makeOk(req, r.result));
    } else {
        sendResponse(socket, makeErr(req, r.errorCode, r.errorMessage, r.errorDetails));
    }
}

} // namespace Nutmeg
