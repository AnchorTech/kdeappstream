#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QList>

#include "QWsServer.h"

class WebSocketServer : public QWsServer
{
        Q_OBJECT
        QWsServer * server;
        QList<QWsSocket*> clients;

    public:

        WebSocketServer(int port);

        ~WebSocketServer();

    signals:

        void serverClosed();

    public slots:

        void newClient();
        void onDataReceived(QString data);
        void onPong(quint64 elapsedTime);
        void onClientDisconnection();

};

#endif // WEBSOCKETSERVER_H
