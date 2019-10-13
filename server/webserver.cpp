#include <QWebSocket>
#include <QtCore/QDebug>
#include <QJsonDocument>

#include "webserver.hpp"
#include "Exception.hpp"

QT_USE_NAMESPACE

WebServer::WebServer(QCoreApplication &a, quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"), QWebSocketServer::NonSecureMode, this)),
    m_client(), m_clients(), bounds(std::make_pair(1, 100)), limit(0), JsonByte(new JsonFormatByte())
{    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
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

void WebServer::checkNumber(void)
{
    if (doc.contains("number") && doc["number"] != "-1" && doc["Play"] == "Running") {
        printf("LOL\n");
        if (doc["number"].toInt() < nbToFind)
            doc["GoodNumber"] = "more";
        else if (doc["number"].toInt() > nbToFind)
            doc["GoodNumber"] = "less";
        else {
            printf("lol\n");
            doc["GoodNumber"] = "OK";
            doc["Play"] = "Finished";
        }
    }
}

void WebServer::checkTentative(void)
{
    if (doc.contains("nbofTentative") && doc["Play"] != "Finished") {
        if (limit == -1) {
            doc["nbofTentative"] = "infini";
            doc["Play"] = "Running";
            return;
        }
        else if (doc["nbofTentative"].toInt() >= -1) {
            if (doc["nbofTentative"].toInt() == -1)
                doc["nbofTentative"] = limit;
            if (doc["Play"].toString() == "Running") {
                doc["nbofTentative"] = doc["nbofTentative"].toInt() - 1;
            }
            if (doc["nbofTentative"].toInt() == 0)
                doc["Play"] = "Finished";
            else
                doc["Play"] = "Running";
        }
        else {
            doc["nbofTentative"] = "0";
            doc["Play"] = "Finished";
        }
    }
}

void WebServer::readFile(void)
{
    QFile JsonFile(QDir::currentPath() + "/save.json");
    QString val;

    if (JsonFile.open(QFile::ReadOnly | QFile::WriteOnly)) {
        QJsonDocument read = QJsonDocument::fromJson(JsonFile.readAll());//QJsonDocument::fromJson(val.toUtf8());
        QJsonObject test = read.object();
        QJsonArray array;
        array << doc;
        read.setArray(array);
        printf("%s\n", QString(read.toJson()).toLocal8Bit().data());
        JsonFile.write(read.toJson());
    }
}

void WebServer::checkFinished(void)
{
    printf("%s\n", doc["Play"].toString().toLocal8Bit().data());
    if (doc["Play"] == "Finished") {
        doc["StopTime"] = QDate::currentDate().toString()+ " " + QTime::currentTime().toString();
        if (doc["GoodNumber"] != "OK")
            doc["HowInput"] = "Looser";
        else
            doc["HowInput"] = "Winner";
        readFile();
    }
}

void WebServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    doc = JsonByte->deserialization(message);
    checkNumber();
    checkTentative();
    checkFinished();

    if (pClient) {
        pClient->sendBinaryMessage(JsonByte->serialization(doc));
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
                QCoreApplication::translate("main", "Limits"), QString("Infini"));
    parser.addOption(limitopt);
    parser.process(a);

    if (parser.value(limitopt) == "Infini")
        setlimit(-1);
    else {
    setlimit(parser.value(limitopt).toInt());
    check_parser(limit, limitopt);
    }
    setbounds(parser.value(boundoptX).toInt(), parser.value(boundoptY).toInt());
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

void WebServer::setnbToFind(int x, int y)
{
    nbToFind = (rand() % y) + x;
    printf("%d\n", nbToFind);
}
