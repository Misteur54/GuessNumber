#include <QWebSocket>
#include <QtCore/QDebug>
#include <QJsonDocument>

#include "webserver.hpp"
#include "Exception.hpp"

QT_USE_NAMESPACE

WebServer::WebServer(QCoreApplication &a, quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"), QWebSocketServer::NonSecureMode, this)),
    m_client(), bounds(std::make_pair(1, 100)), limit(0)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebServer::closed);
        srand(time(NULL));
        initparser(a);
    }
}

WebServer::~WebServer()
{

}

void WebServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &WebServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebServer::socketDisconnected);
    m_client = pSocket;
}

void WebServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        pClient->deleteLater();
    }
}

void WebServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    QJsonObject doc = deserialization(message);
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void WebServer::check_parser(int check, QCommandLineOption &opt)
{
    if (check <= 0 && parser.value(opt).size())
        throw vivoka::Exception("not good limit\n");//+ std::string(argv_limit.toLocal8Bit().data());
}

void WebServer::initparser(QCoreApplication &a)
{
    parser.setApplicationDescription("Plus ou Moins by Elian Server");
    parser.addHelpOption();

    QCommandLineOption boundoptX(QStringList() << "bX" << "boundsX",
                                QCoreApplication::translate("main", "Bounds options X[default: 1]."),
                                QCoreApplication::translate("main", "X"), QString("1"));
    parser.addOption(boundoptX);
    QCommandLineOption boundoptY(QStringList() << "bY" << "boundsY",
                                QCoreApplication::translate("main", "Bounds options Y[default: 100]."),
                                QCoreApplication::translate("main", "Y"), QString("100"));
    parser.addOption(boundoptY);
    QCommandLineOption limitopt(QStringList() << "l" << "limit",
                QCoreApplication::translate("main", "Limits of tentative"),
                QCoreApplication::translate("main", "Limits"));
    parser.addOption(limitopt);
    parser.process(a);

    setlimit(parser.value(limitopt).toInt());
    setbounds(parser.value(boundoptX).toInt(), parser.value(boundoptY).toInt());
    check_parser(limit, limitopt);
    check_parser(bounds.first, boundoptX);
    check_parser(bounds.second, boundoptY);

    if (bounds.first >= bounds.second) {
        throw vivoka::Exception("Bound Y must be higher than bound X");
    }
    setnbToFind(bounds.first, bounds.second);
}

void WebServer::setlimit(int init_limit)
{
    limit = init_limit;
}

void WebServer::setbounds(int x, int y)
{
    bounds = std::make_pair(x, y);

}

QByteArray WebServer::serialization(QJsonObject message)
{
    QJsonDocument doc(message);
    QByteArray bytes = doc.toJson();

    return (bytes);
}

QJsonObject WebServer::deserialization(QByteArray message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message);
    printf("|%s|\n", QString(message).toLocal8Bit().data());
    QJsonObject test = doc.object();
    return (test);
}

void WebServer::setJson()
{
}

void WebServer::setnbToFind(int x, int y)
{
    nbToFind = (rand() % y) + x;
}
