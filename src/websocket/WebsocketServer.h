#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "QWsServer.h"
#include "QWsSocket.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);
    void sendMessage(QString message);

private slots:
    void onConnection();
    void onDisconnection();
    void onDataReceived(QString data);

signals:
    void dataReceived(QString data);

private:
    QWsServer *server;
    QWsSocket *client;
    static const int port = 1234;
};

#endif // SERVER_H
