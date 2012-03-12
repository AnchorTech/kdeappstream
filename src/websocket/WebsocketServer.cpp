#include "WebsocketServer.h"
#include "paint/JSONBuilder.h"
#include <QDebug>

using namespace KAppStream;

WebsocketServer * WebsocketServer::m_instance = 0;

WebsocketServer::WebsocketServer(QObject *parent) :
    QObject(parent),
    server(new QWsServer(this)),
    client(NULL)
{
    server->listen(QHostAddress::Any, port); // returns boolean
    server->setMaxPendingConnections(1);
    connect(server, SIGNAL(newConnection()), this, SLOT(onConnection()));
}

WebsocketServer::~WebsocketServer()
{
    m_instance = 0;
}

WebsocketServer * WebsocketServer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new WebsocketServer(parent);
    return m_instance;
}

bool WebsocketServer::connectSocket()
{
    return server->waitForNewConnection(-1);
}

void WebsocketServer::onConnection()
{
    if(client != NULL) {
        // log error
    }
    client = server->nextPendingConnection();
    if(client)  {
        qDebug() << client->write(QString("Hello"));
        qDebug() << "Client connected " << client->errorString();
        connect(client,SIGNAL(disconnected()),this,SLOT(onDisconnection()));
        connect(client,SIGNAL(frameReceived(QString)),this,SLOT(onDataReceived(QString)));
        connect(JSONBuilder::instance(),SIGNAL(readyRead()),this,SLOT(readData()));
    }
    else
        qDebug() << "error " << client->errorString();
    emit connected();
}

void WebsocketServer::onDisconnection()
{
    disconnect(client,SIGNAL(disconnected()),this,SLOT(onDisconnection()));
    disconnect(client,SIGNAL(frameReceived(QString)),this,SLOT(onDataReceived(QString)));
    client->deleteLater();
    client = NULL;
    qDebug() << "Client disconnected";
    emit disconnected();
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
