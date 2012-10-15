#ifndef EVENTCONVERTER_H
#define EVENTCONVERTER_H

#include <QThread>
#include <QLocalServer>
#include <QScriptEngine>

class EventConverter : public QObject
{
        Q_OBJECT

        static EventConverter * m_instance;

        QScriptEngine engine;

        EventConverter();

        Q_DISABLE_COPY(EventConverter)

    public:

        virtual ~EventConverter();

        static EventConverter * instance();

    public slots:

        void parse(const QString & message);

};

#endif // EVENTCONVERTER_H
