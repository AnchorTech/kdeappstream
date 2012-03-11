#ifndef WEBSOCKETTHREAD_H
#define WEBSOCKETTHREAD_H

#include <QThread>
#include <websocket/WebsocketServer.h>

class WebsocketThread : public QThread
{
    Q_OBJECT
public:
    explicit WebsocketThread(QObject *parent = 0);
    virtual void run();

public:
    WebsocketServer *server;
};

#endif // WEBSOCKETTHREAD_H
