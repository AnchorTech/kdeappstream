#include "WebsocketServer.h"
#include "paint/JSONBuilder.h"
#include <QDebug>

using namespace KAppStream;

WebsocketServer::WebsocketServer(QObject *parent) :
    QObject(parent),
    server(new QWsServer(this)),
    client(NULL)
{
    server->listen(QHostAddress::Any, port); // returns boolean
    server->setMaxPendingConnections(1);
    connect(server, SIGNAL(newConnection()), this, SLOT(onConnection()));
}

void WebsocketServer::onConnection()
{
    if(client != NULL) {
        // log error
    }
    client = server->nextPendingConnection();
    client->write(QString("Hello"));
    if(!client)  {
        // log error
    }
    qDebug() << "Client connected";
    connect(client,SIGNAL(disconnected()),this,SLOT(onDisconnection()));
    connect(client,SIGNAL(frameReceived(QString)),this,SLOT(onDataReceived(QString)));
    connect(JSONBuilder::instance(0),SIGNAL(readyRead()),this,SLOT(readData()));
}

void WebsocketServer::onDisconnection()
{
    disconnect(client,SIGNAL(disconnected()),this,SLOT(onDisconnection()));
    disconnect(client,SIGNAL(frameReceived(QString)),this,SLOT(onDataReceived(QString)));
    client->deleteLater();
    client = NULL;
    qDebug() << "Client disconnected";
}

void WebsocketServer::sendMessage(QString message) {
    if(client) {
        client->write(message);
    }
}

void WebsocketServer::onDataReceived(QString data)
{
    qDebug() << "Received data: " << data;
    sendMessage(data); // pong
    emit dataReceived(data);
}

void WebsocketServer::readData() {
    qDebug() << "readData";
    JSONBuilder::instance(0)->flush(client);
    qDebug() << "finishRead";
}
