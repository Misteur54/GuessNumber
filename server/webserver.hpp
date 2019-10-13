#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <QtCore/QObject>
#include <QWebSocketServer>
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore>
#include <ctime>
#include "IPacket.hpp"

class WebServer : public QObject, public IPacket
{
    Q_OBJECT
public:
    explicit WebServer(QCoreApplication &a, quint16 port, QObject *parent = nullptr);
    virtual ~WebServer();
    void setlimit(int init_limit);
    void setbounds(int x, int y);
    void setnbToFind(int x, int y);

Q_SIGNALS:
    void closed();

private slots:
    void onNewConnection();
    void socketDisconnected();
    void processBinaryMessage(QByteArray message);

private:
    QByteArray serialization(QJsonObject message);
    QJsonObject deserialization(QByteArray message);
    void setJson();

    void initparser(QCoreApplication &a);
    void check_parser(int check, QCommandLineOption &option);
    QCommandLineParser parser;
    QWebSocketServer *m_pWebSocketServer;
    QWebSocket *m_client;
    std::pair<int, int> bounds;
    int limit;
    int nbToFind;
};

#endif // WEBSERVER_HPP
