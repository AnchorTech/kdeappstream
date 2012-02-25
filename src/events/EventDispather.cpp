#include "EventDispather.h"

#include <QWidget>
#include <QLocalServer>
#include <QLocalSocket>
#include <QScriptValue>
#include <QScriptEngine>
#include <QMouseEvent>
#include <QApplication>

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

    QScriptValue sc, tmp;
    QScriptEngine engine;

    while (true)
    {
        QByteArray ba = socket->readLine();
        if (ba.isEmpty())
            continue;

        sc = engine.evaluate("(" + QString(ba) + ")");

        if ((tmp = sc.property("press")).isObject())
        {
            qsreal x = tmp.property("x").toNumber();
            qsreal y = tmp.property("y").toNumber();
            QWidget * w = qApp->widgetAt(x, y);
            if (w)
            {
                QMouseEvent * e = new QMouseEvent(QEvent::MouseButtonPress, QPoint(x, y), Qt::LeftButton, Qt::LeftButton, 0);
                qApp->postEvent(w, e);
            }
        }
    }
}
