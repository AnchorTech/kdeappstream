#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "QWsServer.h"
#include "QWsSocket.h"

class WebsocketServer : public QObject
{
    Q_OBJECT
public:
    WebsocketServer(QObject *parent = 0);
public slots:
    void sendMessage(QString message);

private slots:
    void onConnection();
    void onDisconnection();
    void onDataReceived(QString data);
    void readData();

signals:
    void dataReceived(QString data);

public:
    QWsServer *server;
    QWsSocket *client;
    static const int port = 1234;
};

#endif // SERVER_H
