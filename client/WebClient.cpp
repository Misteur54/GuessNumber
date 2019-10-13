#include <QtCore/QDebug>
#include <iostream>
#include "WebClient.hpp"

QT_USE_NAMESPACE

EchoClient::EchoClient(QCoreApplication &a, bool debug, QObject *parent) :
    QObject(parent),
    m_debug(debug)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &EchoClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &EchoClient::closed);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &EchoClient::MessageReceived);
    initparser(a);
    m_webSocket.open(m_url);
}
void EchoClient::MessageReceived(QByteArray message)
{
    QString question = "";
    QTextStream s(stdin);
    QTextStream out(stdout);
    QJsonObject nb;

    QJsonObject test = deserialization(message);
    while(question.toInt() == 0){
        out << "Enter yout number :" << endl;
        question = s.readLine();
        printf("number is =  %s\n", question.toLocal8Bit().data());
        m_PacketJson["number"] = QString(question).toInt();
    }
    m_webSocket.sendBinaryMessage(serialization(m_PacketJson));
}

void EchoClient::onConnected()
{
    if (m_debug)
        qDebug() << "WebSocket connected";
    m_webSocket.sendBinaryMessage(serialization(m_PacketJson));
}

void EchoClient::sendMessage(QString Message)
{
    QJsonObject jsonObj;
    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    m_webSocket.sendBinaryMessage(Message.toLocal8Bit().data());
}

void EchoClient::initparser(QCoreApplication &a)
{
    printf("test\n");
    parser.setApplicationDescription("Plus ou Moins by Elian Client");
//    parser.addHelpOption();

    QCommandLineOption hostopt(QStringList() << "h" << "host",
                                QCoreApplication::translate("main", "Ip of server [def = 127.0.0.1]"),
                                QCoreApplication::translate("main", "Ip"), QString("127.0.0.1"));
    parser.addOption(hostopt);
    QCommandLineOption portopt(QStringList() << "p" << "port",
                QCoreApplication::translate("main", "Port connection [def = 4242]"),
                QCoreApplication::translate("main", "Port"), QString("1234"));
    parser.addOption(portopt);
    QCommandLineOption autoopt(QStringList() << "a" << "auto",
                                QCoreApplication::translate("main", "Find the number automaticly"),
                                QCoreApplication::translate("main", "Auto"));
    parser.addOption(autoopt);
    QCommandLineOption nameopt(QStringList() << "n" << "name",
                QCoreApplication::translate("main", "Name of Player"),
                QCoreApplication::translate("main", "Name"), QString("Anonymous"));
    parser.addOption(nameopt);
    parser.process(a);

    setName(parser.value(nameopt));
    setAdresse(parser.value(hostopt), parser.value(portopt));
    setAuto(parser.isSet(autoopt));
    setJson();
}

void EchoClient::setName(QString Message)
{
    m_Name = Message;
}

void EchoClient::setAdresse(QString host, QString port)
{
    m_url = "ws://" + host + ":" + port;
}

void EchoClient::setAuto(bool autooption)
{
    m_Ia = autooption;
}

QByteArray EchoClient::serialization(QJsonObject message)
{
    QJsonDocument doc(message);
    QByteArray bytes = doc.toJson();

    return (bytes);
}

QJsonObject EchoClient::deserialization(QByteArray message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message);
    printf("|%s|\n", QString(message).toLocal8Bit().data());
    QJsonObject test = doc.object();
//    QString lol(doc.toJson(QJsonDocument::Compact));
    return (test);
}

void EchoClient::setJson()
{
    m_PacketJson["name"] = m_Name;
    m_PacketJson["number"] = -1;
    m_PacketJson["auto"] = m_Ia;
    m_PacketJson["GoodNumber"] = "NULL";
    m_PacketJson["Finished"] = "NULL";
    m_PacketJson["nbofTentative"] = -1;
}
