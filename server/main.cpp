#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <iostream>
#include <QDebug>
#include "webserver.hpp"
#include "Exception.hpp"


int main(int ac, char **av)
{
    QCoreApplication a(ac, av);
    WebServer *test = new WebServer(1234);

    QCommandLineParser parser;
    parser.setApplicationDescription("Plus ou Moins by Elian");
    parser.addHelpOption();

    QCommandLineOption boundoptX(QStringList() << "bX" << "boundsX",
                                QCoreApplication::translate("main", "Bounds options X[default: 1]."),
                                QCoreApplication::translate("main", "X"));
    parser.addOption(boundoptX);
    QCommandLineOption boundoptY(QStringList() << "bY" << "boundsY",
                                QCoreApplication::translate("main", "Bounds options Y[default: 100]."),
                                QCoreApplication::translate("main", "Y"));
    parser.addOption(boundoptY);
    QCommandLineOption limitopt(QStringList() << "l" << "limit",
                QCoreApplication::translate("main", "Limits of tentative"),
                QCoreApplication::translate("main", "Limits"));
    parser.addOption(limitopt);
    parser.process(a);

    QObject::connect(test, &WebServer::closed, &a, &QCoreApplication::quit);
    try {
        return a.exec();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (84);
    }
}
