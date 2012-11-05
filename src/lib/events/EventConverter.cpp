#include "EventConverter.h"
#include "events/Event.h"
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
#include <QSizeGrip>

EventConverter * EventConverter::m_instance = 0;

EventConverter::EventConverter() :
    engine(this)
{
    m_instance = this;
}

EventConverter::~EventConverter()
{
    m_instance = 0;
}

EventConverter * EventConverter::instance()
{
    if (m_instance)
        return m_instance;
    return new EventConverter();
}

void EventConverter::parse(const QString & message)
{
    QScriptValue value = engine.evaluate("(" + message + ")");
    if (!value.isObject())
        return;

    QString command = value.property("command").toString();

    if (command == "mouse")
    {
        long long id = value.property("id").toNumber();

        QWidget * w = (QWidget*) id;
        if (!WidgetsCollection::instance()->contains(w))
            return;

        int x = value.property("x").toInt32();
        int y = value.property("y").toInt32();
        QString type = value.property("type").toString();
        int buttons = value.property("btn").toInt32();
        int modifiers = value.property("modifiers").toInt32();
        if (type == "move")
        {
            int ox = value.property("ox").toInt32();
            int oy = value.property("oy").toInt32();
            QCoreApplication::postEvent(w, new MouseMoveEvent(QPoint(x,y), QPoint(x,y), QPoint(ox,oy), (Qt::MouseButton) buttons, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers));
        }
        else if (type == "press")
        {
            QCoreApplication::postEvent(w, new MousePressEvent(QPoint(x,y), QPoint(x,y), (Qt::MouseButton) buttons, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers));
        }
        else if (type == "release")
        {
            QCoreApplication::postEvent(w, new MouseReleaseEvent(QPoint(x,y), QPoint(x,y), (Qt::MouseButton) buttons, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers));
        }
        else if (type == "dbclick")
        {
            QCoreApplication::postEvent(w, new MouseDbClickEvent(QPoint(x,y), QPoint(x,y), (Qt::MouseButton) buttons, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers));
        }
    }
    else if (command == "wheel")
    {
        long long id = value.property("id").toNumber();

        QWidget * w = (QWidget*) id;
        if (!WidgetsCollection::instance()->contains(w))
            return;

        int x = value.property("x").toInt32();
        int y = value.property("y").toInt32();
        int buttons = value.property("btn").toInt32();
        int modifiers = value.property("modifiers").toInt32();
        int delta = value.property("delta").toInt32();
        int orientation = value.property("orientation").toInt32();
        QCoreApplication::postEvent(w, new MouseWheelEvent(QPoint(x,y), QPoint(x,y), delta, (Qt::Orientation) orientation, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers));
    }
    else if (command == "resize")
    {
        long long id = value.property("id").toNumber();
        QWidget * w = (QWidget*) id;
        if (!WidgetsCollection::instance()->contains(w))
            return;
        int width = value.property("w").toInt32();
        if (w->minimumWidth() > width)
            width = w->minimumWidth();
        else if (w->maximumWidth() < width)
            width = w->maximumWidth();
        int height = value.property("h").toInt32();
        if (w->minimumHeight() > height)
            height = w->minimumHeight();
        else if (w->maximumHeight() < height)
            height = w->maximumHeight();
        QCoreApplication::postEvent(w, new ResizeEvent(width, height));
    }
    else if (command == "key")
    {
        qDebug() << "\033[35;1mReceived data: \033[0m" << message;

        QWidget * w = QApplication::focusWidget();
        if (!w)
            return;

        if (w->focusProxy())
            w = w->focusProxy();

        int key = value.property("key").toInteger();
        int modifiers = value.property("modifiers").toInteger();
        QString text = QString::fromUtf8(value.property("text").toVariant().toByteArray());
        bool autorep = value.property("autorep").toBool();
        int count = value.property("count").toInteger();

        QString type = value.property("type").toString();
        if (type == "press")
        {
            QCoreApplication::postEvent(w, new QKeyEvent(QEvent::KeyPress, key, (Qt::KeyboardModifier)modifiers, text, autorep, count));
        }
        else if (type == "release")
        {
            QCoreApplication::postEvent(w, new QKeyEvent(QEvent::KeyRelease, key, (Qt::KeyboardModifier)modifiers, text, autorep, count));
        }
    }
    else if (command == "close")
    {
        long long id = value.property("id").toNumber();
        QWidget * w = (QWidget*) id;
        if (!WidgetsCollection::instance()->contains(w))
            return;
        QCoreApplication::postEvent(w, new CloseEvent());
    }
    else if (command == "activate")
    {
        long long id = value.property("id").toNumber();
        QWidget * w = (QWidget*) id;
        if (!WidgetsCollection::instance()->contains(w))
            return;
        //if (!QApplication::activeModalWidget())
            QCoreApplication::postEvent(w, new Activate());
    }
}
