#ifndef NUTMEG_WEBSOCKETSERVER_H
#define NUTMEG_WEBSOCKETSERVER_H

#include <QObject>
#include <QList>
#include <QJsonObject>

class QWebSocketServer;
class QWebSocket;

namespace Nutmeg {

class WebSocketServer : public QObject
{
    Q_OBJECT
  public:
    explicit WebSocketServer(quint16 port, QObject *parent = nullptr);
    ~WebSocketServer() override;

    bool isListening() const;
    quint16 port() const;

  signals:
    void clientConnected(const QString &peer); 
    void clientDisconnected();

  private slots:
    void onNewConnection();
    void processTextMessage(const QString &message);
    void socketDisconnected();

  private:
    QWebSocketServer *m_server;
    QList<QWebSocket*> m_clients;

    // Protocol helpers
    void sendEvent(QWebSocket *socket, const QString &event, const QJsonObject &payload);
    void sendResponse(QWebSocket *socket, const QJsonObject &res);
    QJsonObject makeOk(const QJsonObject &req, const QJsonObject &result);
    QJsonObject makeErr(const QJsonObject &req, const QString &code, const QString &message, const QJsonObject &details = {});
    void handleAction(QWebSocket *socket, const QJsonObject &req);
    QString protocolVersion() const { return QStringLiteral("0.1"); }
};

} // namespace Nutmeg

#endif // NUTMEG_WEBSOCKETSERVER_H
