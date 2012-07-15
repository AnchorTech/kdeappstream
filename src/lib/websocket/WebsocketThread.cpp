#include "WebsocketThread.h"

#include <QEventLoop>
#include <QApplication>

WebsocketThread::WebsocketThread(QObject *parent) :
    QThread(parent),
    server(0)
{

}

quint16 WebsocketThread::serverPort()
{
    if (server)
        return server->serverPort();
    return 0;
}

void WebsocketThread::run()
{
    qDebug() << "preparing connections";
    server = WebsocketServer::instance();
    connect(server, SIGNAL(newConnection()), this, SLOT(onConnection()));
    qDebug() << "WebSocket server started";
    exec();
}

void WebsocketThread::onConnection()
{
    qDebug() << "WebsocketThread::onConnection()";
    emit connected();
}

void WebsocketThread::onDisconnection()
{
    qDebug() << "WebsocketThread::onDisconnection()";
    emit disconnected();
}
