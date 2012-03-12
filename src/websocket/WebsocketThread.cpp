#include "WebsocketThread.h"

#include <QEventLoop>
#include <QApplication>

WebsocketThread::WebsocketThread(QObject *parent) :
    QThread(parent)
{

}

void WebsocketThread::run()
{
    //server = new WebsocketServer();
    qDebug() << "preparing connections";
    //connect(server->server, SIGNAL(newConnection()), this, SLOT(onConnection()));
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
