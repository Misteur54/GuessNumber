#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <QObject>
#include <QWebSocketServer>

class WebServer : public QObject
{
    Q_OBJECT
public:
    explicit WebServer(quint16 port, QObject *parent = nullptr);
    virtual ~WebServer();

Q_SIGNALS:
    void closed();

private slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();
    void processBinaryMessage(QByteArray message);

private:
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket *m_client;
    std::pair<int, int> bounds;
    int limit;
};

#endif // WEBSERVER_HPP
