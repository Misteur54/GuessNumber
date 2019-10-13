#include <QtCore/QDebug>
#include <iostream>
#include <QDateTime>
#include "WebClient.hpp"

QT_USE_NAMESPACE

EchoClient::EchoClient(QCoreApplication &a, bool debug, QObject *parent) :
    QObject(parent),
    m_debug(debug), s(stdin), out(stdout), JsonByte(new JsonFormatByte())
{
    connect(&m_webSocket, &QWebSocket::connected, this, &EchoClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &EchoClient::closed);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &EchoClient::MessageReceived);
    initparser(a);
    m_webSocket.open(m_url);
}

void EchoClient::CheckGoodNumber(void)
{
    if (m_PacketJson.contains("GoodNumber")) {
        if (m_PacketJson["GoodNumber"] == "OK") {
            out << "You win!" << endl;
            m_webSocket.close();
            emit closed();
        } else if (m_PacketJson.contains("number") && m_PacketJson["number"].toInt() != -1)
            out << "It's not good number, try again, it's " << m_PacketJson["GoodNumber"].toString() << endl;
    }
}

void EchoClient::CheckNumberTentative(void)
{
    if (m_PacketJson.contains("nbofTentative")) {
        if (m_PacketJson["nbofTentative"].toInt() == 0 && m_PacketJson["Play"] == "Finished" && m_PacketJson["GoodNumber"] != "OK") {
            out << "You loss because you don't have any change" << endl;
            m_webSocket.close();
            emit closed();
        }
    }
}

void EchoClient::InputUser(void)
{
    QString question = "";

    while (question.toInt() == 0 && m_PacketJson["Play"] == "Running" && m_PacketJson["GoodNumber"] != "OK") {
        out << "Enter yout number :" << endl;
        question = s.readLine();
        m_PacketJson["number"] = QString(question).toInt();
    }
}

void EchoClient::MessageReceived(QByteArray message)
{

    m_PacketJson = JsonByte->deserialization(message);
    CheckGoodNumber();
    CheckNumberTentative();
    InputUser();
    m_webSocket.sendBinaryMessage(JsonByte->serialization(m_PacketJson));
}

void EchoClient::onConnected()
{
    if (m_debug)
        qDebug() << "WebSocket connected";
    m_webSocket.sendBinaryMessage(JsonByte->serialization(m_PacketJson));
}

void EchoClient::initparser(QCoreApplication &a)
{
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

void EchoClient::setJson(void)
{
    m_PacketJson["name"] = m_Name;
    m_PacketJson["number"] = -1;
    m_PacketJson["auto"] = m_Ia;
    m_PacketJson["GoodNumber"] = "NULL";
    m_PacketJson["Play"] = "NULL";
    m_PacketJson["nbofTentative"] = -1;
    m_PacketJson["HowInput"] = "NULL";
    m_PacketJson["HowManyInput"] = 0;
    m_PacketJson["StartTime"] = QDate::currentDate().toString()+ " " + QTime::currentTime().toString();
    m_PacketJson["StopTime"] = "NULL";
}
