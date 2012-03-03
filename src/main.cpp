#include <QtGui/QApplication>

#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QLocalServer>
#include <QLocalSocket>

#include <websocket/HttpServer.h>
#include <websocket/WebsocketServer.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HttpServer htttpServer(8888,NULL);
    Server webSocketServer(NULL);

    QLocalServer server;
    server.setMaxPendingConnections(1);
    server.listen("kappstream_server");

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_PRELOAD", "/usr/local/lib/libkappstream.so");
    env.insert("GAMMARAY_UNSET_PRELOAD", "1");
    QProcess p;
    p.setProcessEnvironment(env);
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.start("testapp");

    if (server.waitForNewConnection(30000))
    {
        QLocalSocket * con = server.nextPendingConnection();
        QByteArray ba = con->readLine();
        if (ba.isEmpty())
            ba = con->readLine();
        QString sock = QString(ba);
        QLocalSocket s;
        sleep(1);
        s.connectToServer("kappstream_" + QString::number(p.pid()));
        if (s.waitForConnected())
        {
            sleep(3);
            const char * d = "{ \"mouse\" : { \"x\" : \"100\", \"y\" : \"100\", \"type\" : \"move\", \"buttons\" : [\"left\"] } }\n";
            s.write(d);
            qDebug() << s.waitForBytesWritten();
        }
        else
            qDebug() << "kappstream_" + QString::number(p.pid()) << "can't connect to process";
        server.close();
        app.exec();
    }
    else
        qDebug() << "cant receive connection";
    server.close();
}
