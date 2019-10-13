#include <QApplication>
#include <QtCore>
#include <iostream>

#include "Vivoka.hpp"
#include "clientmanager.hpp"

int main(int ac, char **av)
{
    QCoreApplication a(ac, av);
    GuessNumber::ClientManager *client;

    try {
        client = new GuessNumber::ClientManager(a);
        QObject::connect(client, &GuessNumber::ClientManager::closed, &a, QCoreApplication::quit);
        a.exec();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }
    return (0);
}
