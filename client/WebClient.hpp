#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore>
#include "IPacket.hpp"
#include "JsonFormat.hpp"

class EchoClient : public QObject, public IPacket
{
    Q_OBJECT
public:
    explicit EchoClient(QCoreApplication &a, bool debug = false, QObject *parent = Q_NULLPTR);
    void setName(QString Name);
    void setAdresse(QString host, QString port);
    void setAuto(bool autoption);

Q_SIGNALS:
    void closed();
    void finished();

private Q_SLOTS:
    void onConnected();
    void MessageReceived(QByteArray message);

private:
    void setJson(void);
    void initparser(QCoreApplication &a);
    void CheckGoodNumber(void);
    void CheckNumberTentative(void);
    void InputUser(void);

    bool m_Ia;
    bool m_debug;
    QWebSocket m_webSocket;
    QUrl m_url;
    QString m_host;
    QString m_port;
    QString m_Name;
    QJsonObject m_PacketJson;
    QJsonObject m_PacketJsonReceive;
    QCommandLineParser parser;
    QTextStream s;
    QTextStream out;
    JsonFormatByte *JsonByte;
};

#endif // ECHOCLIENT_H
