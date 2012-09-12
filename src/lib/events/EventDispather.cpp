#include "EventDispather.h"
#include "events/EventFilter.h"
#include "hooking/WidgetsCollection.h"

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
#include <QStack>

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
    QStack<QEvent*> eventStack;
    QStack<QWidget*> widgetStack;
    QScriptValue value = engine.evaluate("(" + message + ")");
    if (!value.isObject())
        return;

    QString command = value.property("command").toString();

    qDebug() << "\033[35;1mReceived data: \033[0m" << message;

    if (command == "mouse")
    {
        long long id = value.property("id").toNumber();
        int x = value.property("x").toInt32();
        int y = value.property("y").toInt32();
        QWidget * w = (QWidget*) id;

        if (!WidgetsCollection::instance()->contains(w))
        {
            qDebug() << "ni ma";
            return;
        }

        QString type = value.property("type").toString();
        if (type == "move")
        {
            int ox = value.property("ox").toInt32();
            int oy = value.property("oy").toInt32();
            int buttons = value.property("btn").toInt32();
            int modifiers = value.property("modifiers").toInt32();
            if (w->isEnabled())
            {

                if (w->isEnabled() && (buttons || w->hasMouseTracking()))
                    QCoreApplication::postEvent(w, new QMouseEvent(QEvent::MouseMove, QPoint(x,y), QPoint(5,5), (Qt::MouseButton) buttons, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers));
            }

            if (w->windowFlags() & Qt::WA_Hover)
            {
                QPoint p = QPoint(x,y);
                QPoint op = QPoint(ox,oy);
//                QWidget * _w = w;
//                do
//                {
//                    eventStack.push(new QHoverEvent(QEvent::HoverMove, p, op));
//                    widgetStack.push(_w);
//                    QWidget * tmp = _w;
//                    _w = _w->parentWidget();
//                    if (_w)
//                    {
//                        p = tmp->mapToParent(p);
//                        op = tmp->mapToParent(op);
//                    }
//                }
//                while (_w);

                while (!eventStack.isEmpty())
                    QCoreApplication::postEvent(w, new QHoverEvent(QEvent::HoverMove, p, op));
            }
        }
        else if (type == "enter")
        {
            QCoreApplication::postEvent(w, new QEvent(QEvent::Enter));
            if (w->testAttribute(Qt::WA_Hover))
                QCoreApplication::postEvent(w, new QHoverEvent(QEvent::HoverEnter, QPoint(5,5), QPoint(-1,-1)));
        }
        else if (type == "leave")
        {
            QCoreApplication::postEvent(w, new QEvent(QEvent::Leave));
            if (w->testAttribute(Qt::WA_Hover))
                QCoreApplication::postEvent(w, new QHoverEvent(QEvent::HoverLeave, QPoint(-1,-1), QPoint(5,5)));
        }
        else if (type == "press")
        {
            int button = value.property("btn").toInt32();
            int modifiers = value.property("modifiers").toInt32();
            if (w->isEnabled() && button)
                QCoreApplication::postEvent(w, new QMouseEvent(QEvent::MouseButtonPress, QPoint(x,y), QPoint(x,y), (Qt::MouseButton) button, (Qt::MouseButtons) button, (Qt::KeyboardModifiers) modifiers));
        }
        else if (type == "release")
        {
            int button = value.property("btn").toInt32();
            int modifiers = value.property("modifiers").toInt32();
            if (w->isEnabled() && button)
                QCoreApplication::postEvent(w, new QMouseEvent(QEvent::MouseButtonRelease, QPoint(x,y), QPoint(x,y), (Qt::MouseButton) button, (Qt::MouseButtons) button, (Qt::KeyboardModifiers) modifiers));
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
