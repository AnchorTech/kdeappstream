#include "WebsocketThread.h"

WebsocketThread::WebsocketThread(QObject *parent) :
    QThread(parent)
{

}

void WebsocketThread::run() {
    server = new WebsocketServer(0);
    exec();
}
