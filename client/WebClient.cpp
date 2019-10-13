#include <QtCore/QDebug>
#include <iostream>
#include <QDateTime>
#include "WebClient.hpp"
#include "Exception.hpp"

QT_USE_NAMESPACE

GuessNumber::WebClient::WebClient(QCoreApplication &a, QObject *parent) :
    QObject(parent), s(stdin), out(stdout), JsonByte(new JsonFormatByte())
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebClient::closed);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &WebClient::MessageReceived);
    initparser(a);
    srand(time(NULL));
    m_webSocket.open(m_url);
}


void GuessNumber::WebClient::CheckGoodNumber(void)
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

void GuessNumber::WebClient::CheckNumberTentative(void)
{
    if (m_PacketJson.contains("nbofLimit")) {
        if (m_PacketJson["nbofLimit"].toInt() == 0 && m_PacketJson["Play"] == "Finished" && m_PacketJson["GoodNumber"] != "OK") {
            out << "You loss because you don't have any chance" << endl;
            m_webSocket.close();
            emit closed();
        }
    }
}

void GuessNumber::WebClient::InputUser(void)
{
    QString question = "";

    if (m_PacketJson["auto"] == true)
        m_PacketJson["number"] = rand();
    else {
        while (question.toInt() == 0 && m_PacketJson["Play"] == "Running" && m_PacketJson["GoodNumber"] != "OK") {
            out << "Enter one number :" << endl;
            question = s.readLine();
            m_PacketJson["number"] = QString(question).toInt();
        }
        m_PacketJson["HowManyInput"] = m_PacketJson["HowManyInput"].toInt() + 1;
    }
}

void GuessNumber::WebClient::MessageReceived(QByteArray message)
{

    m_PacketJson = JsonByte->deserialization(message);
    CheckGoodNumber();
    CheckNumberTentative();
    InputUser();
    m_webSocket.sendBinaryMessage(JsonByte->serialization(m_PacketJson));
}

void GuessNumber::WebClient::onConnected()
{
   m_webSocket.sendBinaryMessage(JsonByte->serialization(m_PacketJson));
}

void GuessNumber::WebClient::initparser(QCoreApplication &a)
{
    parser.setApplicationDescription("Plus ou Moins by Elian Client");
//    parser.addHelpOption();

    QCommandLineOption hostopt(QStringList() << "h" << "host",
                                QCoreApplication::translate("main", "Ip of server [def = 127.0.0.1]"),
                                QCoreApplication::translate("main", "Ip"), QString("127.0.0.1"));
    parser.addOption(hostopt);
    QCommandLineOption portopt(QStringList() << "p" << "port",
                QCoreApplication::translate("main", "Port connection [def = 4242]"),
                QCoreApplication::translate("main", "Port"), QString("4242"));
    parser.addOption(portopt);
    QCommandLineOption autoopt(QStringList() << "a" << "auto",
                                QCoreApplication::translate("main", "Find the number automaticly"));
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

void GuessNumber::WebClient::setName(QString Message)
{
    m_Name = Message;
}

void GuessNumber::WebClient::setAdresse(QString host, QString port)
{
    m_url = "ws://" + host + ":" + port;
}

void GuessNumber::WebClient::setAuto(bool autooption)
{
    m_Ia = autooption;
}

void GuessNumber::WebClient::setJson(void)
{
    m_PacketJson["name"] = m_Name;
    m_PacketJson["number"] = -1;
    m_PacketJson["auto"] = m_Ia;
    m_PacketJson["GoodNumber"] = "NULL";
    m_PacketJson["Play"] = "NULL";
    m_PacketJson["nbofLimit"] = -1;
    m_PacketJson["HowInput"] = "NULL";
    m_PacketJson["HowManyInput"] = 0;
    m_PacketJson["StartTime"] = QDate::currentDate().toString()+ " " + QTime::currentTime().toString();
    m_PacketJson["StopTime"] = "NULL";
}
