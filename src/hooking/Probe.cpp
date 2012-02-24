#include <QtCore>
#include <dlfcn.h>

class EventFilter;

static EventFilter *evFilter = 0;

class EventFilter : public QObject
{
    bool eventFilter( QObject * recv, QEvent * e )
    {
        qDebug() << recv << e << "eventFiltering";
        return false;
    }
};

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
#if !defined Q_OS_WIN && !defined Q_OS_MAC
    static void(*next_qt_startup_hook)() = (void (*)()) dlsym(RTLD_NEXT, "qt_startup_hook");
    next_qt_startup_hook();
#endif
}
