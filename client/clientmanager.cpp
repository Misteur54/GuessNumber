#include "clientmanager.hpp"
#include <QtCore/QDebug>

QT_USE_NAMESPACE
#include <iostream>

ClientManager::ClientManager(QCoreApplication &a, bool debug, QObject *parent) :
    QObject(parent)
{
    client = new EchoClient(a, false);
}
