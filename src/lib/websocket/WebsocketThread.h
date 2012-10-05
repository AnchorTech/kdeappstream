#ifndef WEBSOCKETTHREAD_H
#define WEBSOCKETTHREAD_H

#include <QThread>
#include <QEventLoop>
#include <websocket/WebsocketServer.h>
#include <paint/ImagesHostServer.h>

class WebsocketThread : public QThread
{
        Q_OBJECT

        WebsocketServer * server;
        ImagesHostServer * imagesServer;

    public:

        explicit WebsocketThread(QObject * parent = 0);
        quint16 serverPort();
        bool waitForConnected(int wait);

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
