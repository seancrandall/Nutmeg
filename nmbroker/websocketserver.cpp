#include "websocketserver.h"

#include <QWebSocketServer>
#include <QWebSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>
#include <QDebug>

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

    if (action == QLatin1String("ping")) {
        QJsonObject result{
            {"pong", true},
            {"now", QDateTime::currentDateTimeUtc().toString(Qt::ISODate)}
        };
        sendResponse(socket, makeOk(req, result));
        return;
    }

    if (action == QLatin1String("info.get")) {
        QJsonObject result{
            {"service", QStringLiteral("NutmegBroker")},
            {"apiVersion", protocolVersion()}
        };
        sendResponse(socket, makeOk(req, result));
        return;
    }

    sendResponse(socket, makeErr(req, QStringLiteral("ENOACTION"), QStringLiteral("Unknown action '") + action + QStringLiteral("'")));
}

} // namespace Nutmeg
