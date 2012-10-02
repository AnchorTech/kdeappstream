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
#include <QSizeGrip>

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
        long long id = value.property("id").toNumber();
        int x = value.property("x").toInt32();
        int y = value.property("y").toInt32();
        QWidget * w = (QWidget*) id;

        if (!WidgetsCollection::instance()->contains(w))
            return;

        //if (dynamic_cast<QSizeGrip*>(w))
        //    return;

        QString type = value.property("type").toString();
        if (type == "move")
        {
            int ox = value.property("ox").toInt32();
            int oy = value.property("oy").toInt32();
            int buttons = value.property("btn").toInt32();
            int modifiers = value.property("modifiers").toInt32();
            if (w->isEnabled())
            {
                if (buttons || w->hasMouseTracking())
                    QCoreApplication::postEvent(w, new QMouseEvent(QEvent::MouseMove, QPoint(x,y), QPoint(5,5), (Qt::MouseButton) buttons, (Qt::MouseButtons) buttons, (Qt::KeyboardModifiers) modifiers));
            }

            if (w->testAttribute(Qt::WA_Hover))
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

                //while (!eventStack.isEmpty())
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
            {
                if (w->focusProxy())
                    w = w->focusProxy();
                if (w->focusPolicy() & Qt::ClickFocus)
                {
                    QWidget * prevW = QApplication::focusWidget();
                    if (prevW)
                        QCoreApplication::postEvent(prevW, new QFocusEvent(QEvent::FocusOut, Qt::MouseFocusReason));
                    QCoreApplication::postEvent(w, new QFocusEvent(QEvent::FocusIn, Qt::MouseFocusReason));
                }
                QCoreApplication::postEvent(w, new QMouseEvent(QEvent::MouseButtonPress, QPoint(x,y), QPoint(x,y), (Qt::MouseButton) button, (Qt::MouseButtons) button, (Qt::KeyboardModifiers) modifiers));

//                {
//                    w->focusProxy()->setFocus(Qt::MouseFocusReason);
//                    QWidget * prevW = QApplication::focusWidget();
//                    if (prevW)
//                        QCoreApplication::postEvent(prevW, new QFocusEvent(QEvent::FocusOut, Qt::MouseFocusReason));
//                    QCoreApplication::postEvent(w->focusProxy(), new QFocusEvent(QEvent::FocusIn, Qt::MouseFocusReason));
//                }
//                else if (w->focusPolicy() & Qt::ClickFocus)
//                {
//                    QWidget * prevW = QApplication::focusWidget();
//                    if (prevW)
//                        QCoreApplication::postEvent(prevW, new QFocusEvent(QEvent::FocusOut, Qt::MouseFocusReason));
//                    QCoreApplication::postEvent(w, new QFocusEvent(QEvent::FocusIn, Qt::MouseFocusReason));
//                }
            }
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
        QWidget * w = (QWidget*) id;
        if (!WidgetsCollection::instance()->contains(w))
        {
            qDebug() << "ni ma widgeta";
            return;
        }
        int width = value.property("w").toInt32();
        if (w->minimumWidth() > width || w->maximumWidth() < width)
        {
            qDebug() << "zły width";
            return;
        }
        int height = value.property("h").toInt32();
        if (w->minimumHeight() > height || w->maximumHeight() < height)
        {
            qDebug() << "zły height";
            return;
        }
        w->resize(width, height);
        w->update();
        //qDebug() << "Poszło!" << QSize(width, height) << w->size();
        //QEvent * e = new QResizeEvent(QSize(width, height), w->size());
        //QCoreApplication::postEvent(w, e);
    }
    else if (command == "move")
    {
        long long id = value.property("id").toNumber();
    }
    else if (command == "key")
    {
        QWidget * w = QApplication::focusWidget();
        if (!w)
        {
            qDebug() << "No focusWidget() !";
            return;
        }
        if (w->focusProxy())
        {
            w = w->focusProxy();
            qDebug() << "focusProxy() ! << w";
        }
        else
        {
            qDebug() << "No focusProxy() !";
        }

        char key = (char) value.property("key").toInteger();
        int modifiers = value.property("modifiers").toInteger();

        QString type = value.property("type").toString();
        if (type == "press")
        {
            char t[] = { key, '\0' };
            QCoreApplication::postEvent(w, new QKeyEvent(QEvent::KeyPress, key, (Qt::KeyboardModifier)modifiers, QString::fromAscii(t)));
        }
        else if (type == "release")
        {
            QCoreApplication::postEvent(w, new QKeyEvent(QEvent::KeyRelease, key, (Qt::KeyboardModifier)modifiers));
        }
    }
    else if (command == "close")
    {
        long long id = value.property("id").toNumber();
        QWidget * w = (QWidget*) id;
        if (!WidgetsCollection::instance()->contains(w))
            return;
        QCoreApplication::postEvent(w, new QCloseEvent());
    }
}
