#include <QtGui/QApplication>

#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QLocalServer>
#include <QLocalSocket>

#include "HttpService.h"
#include "ACLProvider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("kappstream");
    app.setOrganizationName("kde");

    if (argc == 1)
    {
        while (true)
        {
            HttpService service;
            int result = app.exec();
            if (result != 2)
                return result;
        }
    }
    else if (argc == 3)
    {
        if (!strcmp(argv[1], "accept") || !strcmp(argv[1], "a"))
            ACLProvider::instance()->accept(QString::fromAscii(argv[2]));
        else if (!strcmp(argv[1], "reject") || !strcmp(argv[1], "r"))
            ACLProvider::instance()->reject(QString::fromAscii(argv[2]));
        ACLProvider::instance()->printInfo();
    }

//    QLocalServer localSocket;

//    QLocalServer server;
//    server.setMaxPendingConnections(1);
//    server.listen("kappstream_server");

//    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//    env.insert("LD_PRELOAD", "/usr/local/lib64/libkappstream.so");
//    env.insert("GAMMARAY_UNSET_PRELOAD", "1");
//    QProcess p;
//    p.setProcessEnvironment(env);
//    p.setProcessChannelMode(QProcess::ForwardedChannels);
//    p.start("testapp");

//    if (server.waitForNewConnection(30000))
//    {
//        QLocalSocket * con = server.nextPendingConnection();
//        QByteArray ba = con->readLine();
//        if (ba.isEmpty())
//            ba = con->readLine();
//        QString sock = QString(ba);
//        QLocalSocket s;
//        sleep(1);
//        s.connectToServer("kappstream_" + QString::number(p.pid()));
//        s.waitForConnected();

//        server.close();
//    }
//    else
//        qDebug() << "cant receive connection";
//    server.close();
}
