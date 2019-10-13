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
#include "JsonFormat.hpp"

namespace GuessNumber {

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
        void setJson();
        void initparser(QCoreApplication &a);
        void check_parser(int check, QCommandLineOption &option);
        void checkNumber(void);
        void checkTentative(void);
        void checkFinished(void);
        void readFile(void);

        QCommandLineParser parser;
        QWebSocketServer *m_pWebSocketServer;
        QWebSocket *m_client;
        std::pair<int, int> bounds;
        int limit;
        int nbToFind;
        QJsonObject doc;
        JsonFormatByte *JsonByte;
    };
}
#endif // WEBSERVER_HPP
