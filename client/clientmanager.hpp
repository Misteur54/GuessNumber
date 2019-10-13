#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore>
#include "WebClient.hpp"

    namespace GuessNumber {

    class ClientManager : public QObject
    {
        Q_OBJECT
    public:
        explicit ClientManager(QCoreApplication &a, QObject *parent = Q_NULLPTR);

    Q_SIGNALS:
        void closed();

    private:
         GuessNumber::WebClient *client;
    signals:
        void finished();
    };
}
#endif // WebClient_H
