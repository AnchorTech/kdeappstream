#ifndef EVENTDISPATHER_H
#define EVENTDISPATHER_H

#include <QThread>
#include <QLocalServer>

class EventDispather : public QObject
{
        Q_OBJECT

        static EventDispather * m_instance;

        EventDispather();

        Q_DISABLE_COPY(EventDispather)

    public:

        virtual ~EventDispather();

        static EventDispather * instance();

    public slots:

        void parse(const QString & message);

};

#endif // EVENTDISPATHER_H
