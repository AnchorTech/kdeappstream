#include "EventDispather.h"
#include "events/EventFilter.h"

#include <QDebug>
#include <QWidget>
#include <QDesktopWidget>
#include <QLocalServer>
#include <QLocalSocket>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QScriptEngine>
#include <QMouseEvent>
#include <QApplication>

EventDispather * EventDispather::m_instance = 0;

EventDispather::EventDispather() :
    engine(this)
{
    m_instance = this;
}

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

void EventDispather::parse(const QString & message)
{
    QScriptValue value = engine.evaluate("(" + message + ")");
    if (!value.isObject())
        return;

    QString command = value.property("command").toString();

    qDebug() << "\033[35;1mReceived data: \033[0m" << message;

    if (command == "mouse")
    {
        // TODO: widget id validation
        long long id = value.property("id").toNumber();
        int x = value.property("x").toInt32();
        int y = value.property("y").toInt32();
        QWidget * w = (QWidget*) id;
        QString type = value.property("type").toString();
        if (type == "move")
        {
            if (w->isEnabled())
            {
                int buttons = value.property("btn").toInt32();
                int modifiers = value.property("modifiers").toInt32();
                if (w->isEnabled())// && (buttons || w->hasMouseTracking()))
                    QCoreApplication::postEvent(w, new QMouseEvent(QEvent::MouseMove, QPoint(x,y), QPoint(x,y), (Qt::MouseButton) buttons, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers));
            }
            int ox = value.property("ox").toInt32();
            int oy = value.property("oy").toInt32();
            if (w->windowFlags() & Qt::WA_Hover)
                QCoreApplication::postEvent(w, new QHoverEvent(QEvent::HoverMove, QPoint(x,y), QPoint(ox,oy)));
        }
        else if (type == "enter")
        {
            QCoreApplication::postEvent(w, new QEvent(QEvent::Enter));
            if (w->testAttribute(Qt::WA_Hover))
                QCoreApplication::postEvent(w, new QHoverEvent(QEvent::HoverEnter, QPoint(5,5), QPoint(-1,-1)));
        }
        else if (type == "leave")
        {
            //QCoreApplication::postEvent(w, new QEvent(QEvent::Leave));
            //if (w->testAttribute(Qt::WA_Hover))
            //    QCoreApplication::postEvent(w, new QHoverEvent(QEvent::HoverLeave, QPoint(-1,-1), QPoint(5,5)));
        }
        else if (type == "press")
        {
        }
        else if (type == "release")
        {
        }
        else if (type == "dbclick")
        {
        }
    }
    else if (command == "wheel")
    {
        long long id = value.property("id").toNumber();
    }
    else if (command == "resize")
    {
        long long id = value.property("id").toNumber();
    }
    else if (command == "move")
    {
        long long id = value.property("id").toNumber();
    }
    else if (command == "key")
    {
        QString type = value.property("type").toString();
        if (type == "press")
        {
        }
        else if (type == "release")
        {
        }
    }
}
