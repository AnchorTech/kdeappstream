#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <QObject>
#include <QPoint>

class EventDispatcher : public QObject
{
    public:

        EventDispatcher(QObject * parent = 0);
        bool eventFilter(QObject * recv, QEvent * e);

    private:

        void processEnterEvents(QWidget * mouseGrabber, const QPoint & p);

};

#endif // EVENTDISPATCHER_H
