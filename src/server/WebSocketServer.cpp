#include "WebSocketServer.h"

#include <QDebug>
#include <QProcessEnvironment>

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
    env.insert("LD_PRELOAD", "/usr/local/lib64/libkappstream.so");
    env.insert("GAMMARAY_UNSET_PRELOAD", "1");
    QProcess p;
    p.setProcessEnvironment(env);
    p.setProcessChannelMode(QProcess::ForwardedChannels);
    p.start("testapp");

    socket->close("Connect to port 5890 to interact with application");

    this->close();
    emit serverClosed();

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
