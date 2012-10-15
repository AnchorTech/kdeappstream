#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <QObject>
#include <QPoint>

class EventDispatcher : public QObject
{
        static bool _isActivateEvent;

    public:

        EventDispatcher(QObject * parent = 0);
        bool eventFilter(QObject * recv, QEvent * e);
        static bool isActivateEvent()
        {
            return _isActivateEvent;
        }

    private:

        void processEnterEvents(QWidget * mouseGrabber, const QPoint & p);

};

#endif // EVENTDISPATCHER_H
