#include <QApplication>
#include <QtCore/QCoreApplication>
#include <iostream>
#include <QDebug>
#include "webserver.hpp"
#include "Exception.hpp"


int main(int ac, char **av)
{
    QCoreApplication a(ac, av);
    WebServer *server;

    try {
        server = new WebServer(a, 1234);
        QObject::connect(server, &WebServer::closed, &a, &QCoreApplication::quit);
        return a.exec();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }
}
