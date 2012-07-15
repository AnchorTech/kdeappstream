#include "WebSocketService.h"

#include "WebSocketServer.h"

WebSocketService::WebSocketService(int port, QObject * parent) :
    QThread(parent),
    port(port)
{
}

void WebSocketService::run()
{
    WebSocketServer server(port);
    //connect(&server, SIGNAL(serverClosed()), this, SLOT(quit()));
    exec();
    qDebug("End WebSocket service");
}
