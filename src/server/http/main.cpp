#include <QtGui/QApplication>

#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QLocalServer>
#include <QLocalSocket>

#include "HttpService.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HttpService service;
    return app.exec();

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
