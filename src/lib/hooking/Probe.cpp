#include <QtCore>
#include <QSharedMemory>
#include <QLocalSocket>
#include <dlfcn.h>
#include <QWidget>

#include "paint/PaintDevice.h"
#include "paint/PaintEngine.h"
#include "events/EventFilter.h"
#include "events/EventDispather.h"
#include "websocket/WebsocketThread.h"

using namespace KAppStream;

QString getSocketName()
{
    static QString result = "kappstream_" + QString::number(qApp->applicationPid());
    return result;
}

static EventFilter *evFilter = 0;

void installUIExtractorEventFilter()
{
    if ( !evFilter )
        evFilter = new EventFilter();
    qDebug( "Installing UI Extractor event filter." );
    qApp->installEventFilter( evFilter );
}

extern "C" void Q_CORE_EXPORT qt_startup_hook()
{
    WebsocketThread * websocketService = new WebsocketThread(0);
    websocketService->start();
    while (!websocketService->serverPort())
        sleep(0);

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
    socket.write(QString::number(websocketService->serverPort()).toAscii());
    socket.waitForBytesWritten();
    socket.close();

    if (!websocketService->waitForConnected(10000))
        exit(-1);

    installUIExtractorEventFilter();
    //QObject::connect(websocketService, SIGNAL(connected()), evFilter, SLOT(connected()));
    //QObject::connect(websocketService, SIGNAL(disconnected()), evFilter, SLOT(disconnected()));

    //EventDispather::instance("kappstream_entry22", qApp);

#if !defined Q_OS_WIN && !defined Q_OS_MAC
    static void(*next_qt_startup_hook)() = (void (*)()) dlsym(RTLD_NEXT, "qt_startup_hook");
    next_qt_startup_hook();
#endif
}
