#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore>
#include "WebClient.hpp"

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QCoreApplication &a, bool debug = false, QObject *parent = Q_NULLPTR);

Q_SIGNALS:
    void closed();

private:
     EchoClient *client;
signals:
    void finished();
};

#endif // WebClient_H
