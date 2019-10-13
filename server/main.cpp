#include <QApplication>
#include <QtCore/QCoreApplication>
#include <iostream>
#include <QDebug>
#include "webserver.hpp"
#include "Exception.hpp"


int main(int ac, char **av)
{
    QCoreApplication a(ac, av);
    GuessNumber::WebServer *server;

    try {
        server = new GuessNumber::WebServer(a, 4242);
        QObject::connect(server, &GuessNumber::WebServer::closed, &a, &QCoreApplication::quit);
        return a.exec();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }
}
