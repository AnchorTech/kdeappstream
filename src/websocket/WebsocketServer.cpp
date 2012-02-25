#include "WebsocketServer.h"
#include <QDebug>

Server::Server(QObject *parent) :
    QObject(parent),
    server(new QWsServer(this)),
    client(NULL)
{
    server->listen(QHostAddress::Any, port); // returns boolean
    server->setMaxPendingConnections(1);
    connect(server, SIGNAL(newConnection()), this, SLOT(onConnection()));
}

void Server::onConnection()
{
    if(client != NULL) {
        // log error
    }
    client = server->nextPendingConnection();
    connect(client,SIGNAL(connected()),this,SLOT(onConnection()));
    connect(client,SIGNAL(disconnected()),this,SLOT(onDisconnection()));
    connect(client,SIGNAL(frameReceived(QString)),this,SLOT(onDataReceived(QString)));
}

void Server::onDisconnection()
{
    client->deleteLater();
    client = NULL;
}

void Server::onDataReceived(QString data)
{
    qDebug() << "Received data: " << data;
}
