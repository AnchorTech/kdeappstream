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

EventDispather::~EventDispather()
{
    m_instance = 0;
}

EventDispather * EventDispather::instance(const QString & sName, QObject * parent)
{
    if (m_instance)
        return m_instance;
    return new EventDispather(sName, parent);
}

void EventDispather::run()
{
    if(!qApp)
    {
        qDebug() << name << "No application#############################################";
        return;
    }

    qDebug() << name << "HAS APP#############################################";


    QLocalServer server;
    server.setMaxPendingConnections(1);
    if (!server.listen(name) || !server.waitForNewConnection(-1))
    {
        qDebug() << name << "No connection#############################################";

        return;
    }

    qDebug() << name << "HAS CONNECTION #############################################";


    QLocalSocket * socket = server.nextPendingConnection();

    QScriptValue sc, tmp;
    QScriptEngine engine;

    while (true)
    {
        while (!socket->waitForReadyRead())
            qDebug() << "no data";
        qDebug() << "hasdata!";

        QByteArray ba = socket->readLine();
        if (ba.isEmpty())
        {
            qDebug() << "empty data" << ba << "##########################################################";
            continue;
        }

        qDebug() << "HAS DATA" << ba << "##############################################";

        sc = engine.evaluate("(" + QString(ba) + ")");

        qDebug() << "sth evaluated";

        if ((tmp = sc.property("mouse")).isObject())
        {
            qDebug() << "sth good received";
            qsreal x = tmp.property("x").toNumber();
            qsreal y = tmp.property("y").toNumber();
            qDebug() << x << y;
            QWidget * w = QApplication::desktop();
            qDebug() << "some widget" << w;

            if (w)
            {
                QMouseEvent * e = 0;
                QEvent::Type t = QEvent::None;
                Qt::MouseButton b = Qt::NoButton;
                Qt::MouseButtons bs = Qt::NoButton;
                Qt::KeyboardModifiers m = Qt::NoModifier;
                QString type = tmp.property("type").toString();
                if (type.isEmpty())
                    continue;
                if (type == "move") {
                    t = QEvent::MouseMove;
                    qDebug() << "move event";
                }
                else
                {
                    if (type == "press") {
                        t = QEvent::MouseButtonPress;
                        qDebug() << "press event";
                    }
                    else if (type == "release") {
                        t = QEvent::MouseButtonRelease;
                        qDebug() << "release event";
                    }
                    else if (type == "dbclick") {
                        t = QEvent::MouseButtonDblClick;
                        qDebug() << "dbclick event";
                    }
                    else {
                        qDebug() << type;
                        continue;
                    }

                    QScriptValue buttons;
                    if ((buttons = tmp.property("buttons")).isArray())
                    {
                        QScriptValueIterator it (buttons);
                        while (it.hasNext()) {
                            it.next();
                            QString btn = it.value().toString();
                            if (btn == "left")
                            {
                                b = Qt::LeftButton;
                                bs |= Qt::LeftButton;
                            }
                            else if (btn == "right")
                            {
                                b = Qt::RightButton;
                                bs |= Qt::RightButton;
                            }
                            else if (btn == "mid")
                            {
                                b = Qt::MidButton;
                                bs |= Qt::MidButton;
                            }
                            else if (btn == "x1")
                            {
                                b = Qt::XButton1;
                                bs |= Qt::XButton1;
                            }
                            else if (btn == "x2")
                            {
                                b = Qt::XButton2;
                                bs |= Qt::XButton2;
                            }
                        }
                    }

                    if (b == Qt::NoButton && bs == Qt::NoButton)
                        t = QEvent::MouseMove;
                }

                QScriptValue modifiers;
                if ((modifiers = tmp.property("modifiers")).isArray())
                {
                    QScriptValueIterator it (modifiers);
                    while (it.hasNext()) {
                        it.next();
                        QString mod = it.value().toString();

                        if (mod == "shift")
                            m |= Qt::ShiftModifier;
                        else if (mod == "ctrl")
                            m |= Qt::ControlModifier;
                        else if (mod == "alt")
                            m |= Qt::AltModifier;
                        else if (mod == "meta")
                            m |= Qt::MetaModifier;
                        else if (mod == "keypad")
                            m |= Qt::KeypadModifier;
                        else if (mod == "switch")
                            m |= Qt::GroupSwitchModifier;
                    }
                }
                qDebug() << QPoint(x,y);
                qDebug() << QApplication::activeWindow();
                qDebug() << QApplication::activeWindow()->childAt(x,y);
                w = QApplication::activeWindow()->childAt(x,y);
                qDebug() << "to by było na tyle";
                qDebug() << w->mapFromParent(QPoint(x, y));
                e = new QMouseEvent(t, w->mapFromParent(QPoint(x, y)), Qt::NoButton, Qt::NoButton, m);

                qDebug() << "sending event";
                if (e)
                {
                    qApp->postEvent(w, e);
                    QApplication::sendPostedEvents();
                }
            }
        }
        else
            qDebug() << "shit received";
    }
}
