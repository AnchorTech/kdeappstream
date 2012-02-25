#include "EventDispather.h"

#include <QLocalServer>
#include <QLocalSocket>
#include <QScriptValue>
#include <QScriptEngine>

EventDispather * EventDispather::m_instance = 0;

EventDispather::~EventDispather()
{
    m_instance = 0;
}

EventDispather * EventDispather::instance()
{
    if (m_instance)
        return m_instance;
    return new EventDispather();
}

void EventDispather::run()
{
    if(!qApp)
        return;

    QLocalServer server;
    server.setMaxPendingConnections(1);
    if (!server.waitForNewConnection())
        return;

    QLocalSocket * socket = server.nextPendingConnection();

    QScriptValue sc;
    QScriptEngine engine;

    while (true)
    {
        QByteArray ba = socket->readLine();
        if (ba.isEmpty())
            continue;

        sc = engine.evaluate("(" + QString(ba) + ")");

        if (sc.property("result").isObject())
        {
                QStringList items;
                qScriptValueToSequence(sc.property("result"), items);

                foreach (QString str, items) {
                     qDebug("value %s",str.toStdString().c_str());
                 }

        }
    }
}
