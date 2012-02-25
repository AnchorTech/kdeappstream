#ifndef EVENTDISPATHER_H
#define EVENTDISPATHER_H

#include <QThread>

class EventDispather : public QThread
{
        static EventDispather * m_instance;

        EventDispather()
        {
            m_instance = this;
        }

        Q_DISABLE_COPY(EventDispather)

    public:

        virtual ~EventDispather();

        static EventDispather * instance();

    protected:

        virtual void run();

};

#endif // EVENTDISPATHER_H
