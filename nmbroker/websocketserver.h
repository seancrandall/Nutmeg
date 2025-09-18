#ifndef NUTMEG_WEBSOCKETSERVER_H
#define NUTMEG_WEBSOCKETSERVER_H

#include <QObject>
#include <QList>

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
};

} // namespace Nutmeg

#endif // NUTMEG_WEBSOCKETSERVER_H
