#include "WebsocketThread.h"

#include <QEventLoop>
#include <QApplication>

WebsocketThread::WebsocketThread(QObject *parent) :
    QThread(parent),
    server(0),
    imagesServer(0)
{

}

quint16 WebsocketThread::serverPort()
{
    if (server)
        return server->serverPort();
    return 0;
}

quint16 WebsocketThread::imagesServerPort()
{
    if (imagesServer)
        return imagesServer->serverPort();
    return 0;
}

bool WebsocketThread::waitForConnected(int wait)
{
    return server->waitForConnected(wait);
}

void WebsocketThread::run()
{
    qDebug() << "preparing connections";
    server = WebsocketServer::instance();
    connect(server, SIGNAL(newConnection()), this, SLOT(onConnection()));
    qDebug() << "WebSocket server started";
    imagesServer = ImagesHostServer::instance();
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
