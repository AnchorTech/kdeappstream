#ifndef EVENTDISPATHER_H
#define EVENTDISPATHER_H

#include <QThread>

class EventDispather : public QThread
{
        static EventDispather * m_instance;

        QString name;

        EventDispather(const QString & sName, QObject * parent) :
            QThread(parent)
        {
            m_instance = this;
            name = sName;
        }

        Q_DISABLE_COPY(EventDispather)

    public:

        virtual ~EventDispather();

        static EventDispather * instance(const QString & sName, QObject * parent);

    protected:

        virtual void run();

};

#endif // EVENTDISPATHER_H
