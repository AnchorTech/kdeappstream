#include "WebSocketServer.h"

#include <QDebug>
#include <QProcessEnvironment>
#include <QSharedMemory>

WebSocketServer::WebSocketServer(int port) :
    QWsServer()
{
    connect(this, SIGNAL(newConnection()), this, SLOT(newClient()));

    if (this->listen(QHostAddress::Any, port))
    {
        qDebug("WebSocket server started");
    }
    else
    {
        qDebug("Cannot start WebSocket server!");
        qDebug(this->errorString().toAscii());
    }
}

WebSocketServer::~WebSocketServer()
{
}

void WebSocketServer::newClient()
{
    QWsSocket * clientSocket = this->nextPendingConnection();

    connect(clientSocket, SIGNAL(frameReceived(QString)), this, SLOT(onDataReceived(QString)));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(onClientDisconnection()));
    connect(clientSocket, SIGNAL(pong(quint64)), this, SLOT(onPong(quint64)));

    clients << clientSocket;

    qDebug("Client connected");
}

void WebSocketServer::onDataReceived(QString data)
{
    QWsSocket * socket = qobject_cast<QWsSocket*>( sender() );
    if (socket == 0)
        return;

    qDebug("Connection received");
    qDebug(data.toAscii());

    // TODO : Data verification
    Q_UNUSED(data)
    // TODO : Data verification

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LD_PRELOAD", "/usr/lib64/libkappstream.so");
    env.insert("GAMMARAY_UNSET_PRELOAD", "1");
    QProcess p;
    p.setProcessEnvironment(env);
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.start("testapp");

    QSharedMemory sm("kappstream_1234m");

    if (p.waitForStarted())
    {
        qDebug("Process started");
        qDebug(QString("kappstream_1234mm" + QString::number(p.pid())).toAscii());

        sm.create(sizeof(qint64));
        if (!sm.isAttached() && !sm.attach())
        {
            qDebug("Process cannot be synchronized.");
            qDebug(QString::number(sm.error()).toAscii());
            emit serverClosed();
        }

        for (int i = 0; i < 100; ++i)
        {
            if (sm.lock())
            {
                qint64 * port = (qint64 *) sm.data();
                qDebug() << *port;
                if (*port > 0)
                    break;
                sm.unlock();
            }
            sleep(2);
        }
        sm.detach();
    }
    else
    {
        qDebug("Process cannot be started.");
    }

    socket->close("Connect to port 5890 to interact with application");

    this->close();

    qDebug("WebSocket server closed");
}

void WebSocketServer::onPong(quint64 elapsedTime)
{
    qDebug( QString("ping: " + QString::number(elapsedTime) + " ms").toAscii() );
}

void WebSocketServer::onClientDisconnection()
{
    QWsSocket * socket = qobject_cast<QWsSocket*>(sender());
    if (socket == 0)
        return;

    clients.removeOne(socket);

    socket->deleteLater();

    qDebug("Client disconnected");
}
