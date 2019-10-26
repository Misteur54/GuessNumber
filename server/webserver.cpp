#include <QWebSocket>
#include <QtCore/QDebug>
#include <QJsonDocument>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#include "webserver.hpp"
#include "Exception.hpp"

QT_USE_NAMESPACE

GuessNumber::WebServer::WebServer(QCoreApplication &a, quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"), QWebSocketServer::NonSecureMode, this)),
    m_client(), bounds(std::make_pair(1, 100)), limit(0), JsonByte(new JsonFormatByte())
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WebServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebServer::closed);
        srand(time(NULL));
        initparser(a);
    } else
        throw (GuessNumber::Exception("not allocated listen\n"));
}

GuessNumber::WebServer::~WebServer()
{

}

void GuessNumber::WebServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &WebServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebServer::socketDisconnected);
    setnbToFind(bounds.first, bounds.second);
    m_client = pSocket;
}

void GuessNumber::WebServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (pClient) {
        pClient->deleteLater();
    }
}

void GuessNumber::WebServer::checkNumber(void)
{
    if (doc.contains("number") && doc["number"] != "-1" && doc["Play"] == "Running") {
        if (doc["number"].toInt() < nbToFind)
            doc["GoodNumber"] = "more";
        else if (doc["number"].toInt() > nbToFind)
            doc["GoodNumber"] = "less";
        else {
            doc["GoodNumber"] = "OK";
            doc["Play"] = "Finished";
        }
    }
}

void GuessNumber::WebServer::checkTentative(void)
{
    if (doc.contains("nbofLimit") && doc["Play"] != "Finished") {
        if (limit == -1) {
            doc["nbofLimit"] = "infini";
            doc["Play"] = "Running";
            return;
        }
        else if (doc["nbofLimit"].toInt() >= -1) {
            if (doc["nbofLimit"].toInt() == -1)
                doc["nbofLimit"] = limit;
            if (doc["Play"].toString() == "Running") {
                doc["nbofLimit"] = doc["nbofLimit"].toInt() - 1;
            }
            if (doc["nbofLimit"].toInt() == 0)
                doc["Play"] = "Finished";
            else
                doc["Play"] = "Running";
        }
        else {
            doc["nbofLimit"] = "0";
            doc["Play"] = "Finished";
        }
    }
}

void GuessNumber::WebServer::readFile(void)
{
    QFile JsonFile(QDir::currentPath() + "/save.json");
    QJsonDocument write;
    QJsonObject recordObj;

    if (JsonFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::ReadOnly)) {
        recordObj.insert("Party Status", QJsonValue::fromVariant(doc["HowInput"].toString()));
        recordObj.insert("Start Party", QJsonValue::fromVariant(doc["StartTime"].toString()));
        recordObj.insert("Stop Party", QJsonValue::fromVariant(doc["StopTime"].toString()));
        recordObj.insert("Name", QJsonValue::fromVariant(doc["name"].toString()));
        recordObj.insert("Number of Tentative", QJsonValue::fromVariant(doc["HowManyInput"].toInt()));
	write.setObject(recordObj);
        JsonFile.write(write.toJson());
        JsonFile.close();
    } else
        throw (GuessNumber::Exception("not open JsonFile\n"));
}

void GuessNumber::WebServer::checkFinished(void)
{
    if (doc["Play"] == "Finished") {
        doc["StopTime"] = QDate::currentDate().toString()+ " " + QTime::currentTime().toString();
        if (doc["GoodNumber"] != "OK")
            doc["HowInput"] = "Looser";
        else
            doc["HowInput"] = "Winner";
        readFile();
    }
}

void GuessNumber::WebServer::processBinaryMessage(QByteArray message)
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

void GuessNumber::WebServer::check_parser(int check, QCommandLineOption &opt)
{
    if (check <= 0 && parser.value(opt).size())
        throw GuessNumber::Exception("not good limit\n");
}

void GuessNumber::WebServer::initparser(QCoreApplication &a)
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

    if (bounds.first >= bounds.second)
        throw GuessNumber::Exception("Bound Y must be higher than bound X");
    setnbToFind(bounds.first, bounds.second);
}

void GuessNumber::WebServer::setlimit(int init_limit)
{
    limit = init_limit;
}

void GuessNumber::WebServer::setbounds(int x, int y)
{
    bounds = std::make_pair(x, y);
}

void GuessNumber::WebServer::setnbToFind(int x, int y)
{
    nbToFind = rand()%(y - x + 1) + x;
}
