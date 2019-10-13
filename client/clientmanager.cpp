#include <QtCore/QDebug>
#include <iostream>
#include "clientmanager.hpp"

QT_USE_NAMESPACE

GuessNumber::ClientManager::ClientManager(QCoreApplication &a, QObject *parent) :
    QObject(parent)
{
    client = new GuessNumber::WebClient(a);
    QObject::connect(client, &GuessNumber::WebClient::closed, QCoreApplication::quit);
}
