#include <QApplication>
#include "WebClient.hpp"
#include "clientmanager.hpp"

#include <QtCore>
#include <iostream>
int main(int ac, char **av)
{
    QCoreApplication a(ac, av);
    ClientManager *client = new ClientManager(a, false);

    try {
        QObject::connect(client, &ClientManager::closed, &a, QCoreApplication::quit);
        a.exec();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }
    return (0);
}
