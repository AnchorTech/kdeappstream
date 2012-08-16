#include "EventDispather.h"

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

    qDebug() << "Received data: " << message;

    if (command == "mouse")
    {
        // TODO: widget id validation
        long long id = value.property("id").toNumber();
        QString type = value.property("type").toString();
        if (type == "move")
        {
            int x = value.property("x").toInt32();
            int y = value.property("y").toInt32();
            int buttons = value.property("btn").toInt32();
            int modifiers = value.property("modifiers").toInt32();
            QMouseEvent * e = new QMouseEvent(QEvent::MouseMove, QPoint(x,y), (Qt::MouseButton) buttons, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers);
            e->setAccepted(false);
            QCoreApplication::postEvent((QWidget*)id, e, INT_MAX);
        }
        else if (type == "enter")
        {
            QEvent * e = new QEvent(QEvent::Enter);
            e->setAccepted(false);
            QCoreApplication::postEvent((QWidget*)id, e, INT_MAX);
        }
        else if (type == "leave")
        {
            QEvent * e = new QEvent(QEvent::Leave);
            e->setAccepted(false);
            QCoreApplication::postEvent((QWidget*)id, e, INT_MAX);
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
