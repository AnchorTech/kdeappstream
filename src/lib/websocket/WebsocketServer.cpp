#include "WebsocketServer.h"
#include "paint/ImagesBuffer.h"
#include "paint/JSONBuilder.h"
#include "events/EventConverter.h"
#include "events/EventFilter.h"

#include <QDebug>
#include <QCoreApplication>
#include <QHoverEvent>
#include <QWidget>

using namespace KAppStream;

WebsocketServer * WebsocketServer::m_instance = 0;

WebsocketServer::WebsocketServer(QObject *parent) :
    QObject(parent),
    server(new QWsServer(this)),
    client(0),
    connectionSemaphore(0)
{
    server->listen(QHostAddress::Any); // returns boolean
    server->setMaxPendingConnections(1);
    connect(server, SIGNAL(newConnection()), this, SLOT(onConnection()));
    connect(JSONBuilder::instance(), SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this, SIGNAL(sendImage(qreal, QIODevice*)), ImagesBuffer::instance(), SLOT(sendData(qreal, QIODevice*)));
    //connect(this, SIGNAL(disconnected()), qApp, SLOT(quit()));
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

quint16 WebsocketServer::serverPort()
{
    return server->serverPort();
}

bool WebsocketServer::waitForConnected(int wait)
{
    int i = 10;
    int time = wait / 10 * 1000;
    while(--i && !connectionSemaphore.available())
        usleep(time);
    return connectionSemaphore.available();
}

void WebsocketServer::onConnection()
{
    client = server->nextPendingConnection();
    if(client)
    {
        qDebug() << "Client connected ";
        connect(client,SIGNAL(disconnected()),this,SLOT(onDisconnection()));
        connect(client,SIGNAL(frameReceived(QString)),this,SLOT(onDataReceived(QString)));
        if (!connectionSemaphore.available())
            connectionSemaphore.release();
        emit connected();
    }
    else
    {
        qDebug() << "error " << server->errorString();
    }
}

void WebsocketServer::onDisconnection()
{
    if (connectionSemaphore.available())
        connectionSemaphore.acquire();
    disconnect(client,SIGNAL(disconnected()),this,SLOT(onDisconnection()));
    disconnect(client,SIGNAL(frameReceived(QString)),this,SLOT(onDataReceived(QString)));
    client->deleteLater();
    client = NULL;
    qDebug() << "Client disconnected";
    emit disconnected();
    qApp->quit();
}

void WebsocketServer::sendMessage(QString message)
{
    if(client)
        client->write(message);
}

void WebsocketServer::onDataReceived(QString data)
{
    EventConverter::instance()->parse(data);
}

void WebsocketServer::readData()
{
    JSONBuilder::instance(0)->flush(client);
}
