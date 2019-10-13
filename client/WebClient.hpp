#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore>
#include "IPacket.hpp"

class EchoClient : public QObject, public IPacket
{
    Q_OBJECT
public:
    explicit EchoClient(QCoreApplication &a, bool debug = false, QObject *parent = Q_NULLPTR);
    void sendMessage(QString Message);
    void initparser(QCoreApplication &a);
    void setName(QString Name);
    void setAdresse(QString host, QString port);
    void setAuto(bool autoption);

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void MessageReceived(QByteArray message);

signals:
    void finished();
private:
    QByteArray serialization(QJsonObject message);
    QJsonObject deserialization(QByteArray message);
    void setJson();

    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_Ia;
    bool m_debug;
    QString m_host;
    QString m_port;
    QString m_Name;
    QJsonObject m_PacketJson;
    QJsonObject m_PacketJsonReceive;
    QCommandLineParser parser;
};

#endif // ECHOCLIENT_H
