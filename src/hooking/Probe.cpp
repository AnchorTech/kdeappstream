#include <QtCore>
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
    installUIExtractorEventFilter();

    WebsocketThread *websocketThread = new WebsocketThread(0);
    websocketThread->start();
    QLocalSocket socket;
    socket.connectToServer("kappstream_server");
    if (!socket.waitForConnected() || socket.state() == QLocalSocket::UnconnectedState)
        exit(-1);
    qDebug() << socket.write(getSocketName().toAscii()) << getSocketName().toAscii();

    EventDispather::instance(getSocketName(), qApp);

    socket.disconnectFromServer();


#if !defined Q_OS_WIN && !defined Q_OS_MAC
    static void(*next_qt_startup_hook)() = (void (*)()) dlsym(RTLD_NEXT, "qt_startup_hook");
    next_qt_startup_hook();
#endif
}
