#ifndef EVENTDISPATHER_H
#define EVENTDISPATHER_H

#include <QThread>
#include <QLocalServer>

class EventDispather : public QLocalServer
{
        Q_OBJECT

        static EventDispather * m_instance;

        QLocalSocket * socket;

        QString name;

        EventDispather(const QString & sName, QObject * parent);

        Q_DISABLE_COPY(EventDispather)

    public:

        virtual ~EventDispather();

        static EventDispather * instance(const QString & sName, QObject * parent);

    public slots:

        void run();

};

#endif // EVENTDISPATHER_H
