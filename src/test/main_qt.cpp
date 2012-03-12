#include <QtGui/QApplication>

#include "TestWindow.h"
#include "events/EventFilter.h"
#include "websocket/WebsocketThread.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KAppStream::EventFilter * ef;
    app.installEventFilter(ef = new KAppStream::EventFilter);
    ef->connect(WebsocketServer::instance(), SIGNAL(connected()), ef, SLOT(connected()));
    app.connect(WebsocketServer::instance(), SIGNAL(disconnected()), &app, SLOT(quit()));
    if (WebsocketServer::instance()->connectSocket())
    {
        TestWindow w;
        w.show();
        app.exec();
    }
}

