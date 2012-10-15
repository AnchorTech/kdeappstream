#include <QtCore>
#include <QSharedMemory>
#include <QLocalSocket>
#include <dlfcn.h>
#include <QWidget>

#include "paint/PaintDevice.h"
#include "paint/PaintEngine.h"
#include "events/EventFilter.h"
#include "events/EventDispatcher.h"
#include "websocket/WebsocketThread.h"

using namespace KAppStream;

QString getSocketName()
{
    static QString result = "kappstream_" + QString::number(qApp->applicationPid());
    return result;
}

static EventFilter * evFilter = 0;
static EventDispatcher * evDispatcher = 0;

void installUIExtractorEventFilter()
{
    if ( !evFilter )
        evFilter = new EventFilter();
    if ( !evDispatcher )
        evDispatcher = new EventDispatcher();
    qDebug( "Installing UI Extractor event filter." );
    qApp->installEventFilter( evFilter );
    qDebug( "Installing events dispatcher." );
    qApp->installEventFilter( evDispatcher );
}

extern "C" void Q_CORE_EXPORT qt_startup_hook()
{
    WebsocketThread * websocketService = new WebsocketThread(0);
    websocketService->start();
    while (!websocketService->serverPort())
        usleep(100000);
    while (!websocketService->imagesServerPort())
        usleep(100000);

    QLocalSocket socket;
    int i = 0;
    do
    {
        if (i > 0)
        {
            qDebug() << "Repeat connecting to the local init socket";
            qDebug() << socket.errorString();
            sleep(5);
        }
        socket.connectToServer("kappstream_init");
        if (++i > 5)
        {
            qDebug() << "Can't connect to the local init socket";
            exit(-1);
        }
    }
    while(!socket.waitForConnected());

    installUIExtractorEventFilter();

    socket.write((QString::number(websocketService->serverPort()) + " " + QString::number(websocketService->imagesServerPort())).toAscii());
    if (!socket.waitForBytesWritten(30000))
        exit(-1);
    socket.close();

    if (!websocketService->waitForConnected(10000))
        exit(-1);

#if !defined Q_OS_WIN && !defined Q_OS_MAC
    static void(*next_qt_startup_hook)() = (void (*)()) dlsym(RTLD_NEXT, "qt_startup_hook");
    next_qt_startup_hook();
#endif

    qDebug("Connected!");
}
