#ifndef WEBSOCKETTHREAD_H
#define WEBSOCKETTHREAD_H

#include <QThread>
#include <QEventLoop>
#include <websocket/WebsocketServer.h>

class WebsocketThread : public QThread
{
        Q_OBJECT

        WebsocketServer * server;

    public:

        explicit WebsocketThread(QObject * parent = 0);

    public slots:

        void onConnection();
        void onDisconnection();

    signals:

        void connected();
        void disconnected();

    protected:

        virtual void run();

};

#endif // WEBSOCKETTHREAD_H
