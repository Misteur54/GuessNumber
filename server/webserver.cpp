#include "webserver.hpp"
#include <QWebSocket>

WebServer::WebServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"), QWebSocketServer::NonSecureMode, this)),
    m_client()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebServer::closed);
    }
}

WebServer::~WebServer()
{

}

void WebServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

   // connect(pSocket, &QWebSocket::isValid, this, &WebServer::processTextMessage);
    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &WebServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebServer::socketDisconnected);
    m_client = pSocket;
}

void WebServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        printf("msg recu = %s\n", message.toLocal8Bit().data());
        pClient->sendTextMessage("Tu es connecté");
    }
}

void WebServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (pClient) {
     //   m_client->disconnect();
        pClient->deleteLater();
    }
}

void WebServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    printf("je passe là\n");
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}
