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
#include <QtTest>

EventDispather * EventDispather::m_instance = 0;

EventDispather::EventDispather(const QString & sName, QObject * parent) :
    QLocalServer(parent)
{
    m_instance = this;
    name = sName;

    if(!qApp)
    {
        qDebug() << name << "No application#############################################";
        return;
    }

    qDebug() << name << "HAS APP#############################################";

    this->setMaxPendingConnections(1);
    if (!this->listen(name) || !this->waitForNewConnection(-1))
    {
        qDebug() << name << "No connection#############################################";

        return;
    }

    socket = this->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(run()));
}

EventDispather::~EventDispather()
{
    m_instance = 0;
    socket->close();
    this->close();
}

EventDispather * EventDispather::instance(const QString & sName, QObject * parent)
{
    if (m_instance)
        return m_instance;
    return new EventDispather(sName, parent);
}

void EventDispather::run()
{
    qDebug() << name << "HAS CONNECTION #############################################";

    QScriptValue sc, tmp;
    QScriptEngine engine;

    //while (true)
    if (socket->bytesAvailable())
    {
        //while (!socket->waitForReadyRead())
        //    qDebug() << "no data";
        qDebug() << "hasdata!";

        QByteArray ba = socket->readLine();
        if (ba.isEmpty())
        {
            qDebug() << "empty data" << ba << "##########################################################";
            return;
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
                    return;
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
                        return;
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
                QPoint p = QPoint(x,y);
                QWidget * tempw = QApplication::activeWindow();
                while (tempw)
                {
                    qDebug() << tempw << p << tempw->parent();
                    tempw = tempw->childAt(p);
                    if (tempw)
                        p = tempw->mapFromParent(p);
                }
                qDebug() << "-------------------------------";
                p = QPoint(x,y);
                tempw = QApplication::widgetAt(p);
                while (tempw)
                {
                    qDebug() << tempw << p;
                    p = tempw->mapFromParent(p);
                    tempw = tempw->childAt(p);
                }
                qDebug() << "-------------------------------";
                p = QPoint(x,y);
                tempw = QApplication::topLevelAt(p);
                while (tempw)
                {
                    qDebug() << tempw << p;
                    p = tempw->mapFromParent(p);
                    tempw = tempw->childAt(p);
                }
                qDebug() << "-------------------------------";
                qDebug() << QApplication::topLevelWidgets();
                w = QApplication::activeWindow()->childAt(x,y)->parentWidget();
                qDebug() << "to by było na tyle";
                e = new QMouseEvent(QEvent::MouseButtonPress, w->mapFrom(QApplication::activeWindow(), QPoint(x, y)), QApplication::activeWindow()->mapToGlobal(QPoint(x, y)), Qt::NoButton, Qt::NoButton, m);
                qDebug() << e->pos() << e->globalPos();
                //qDebug() << "sending event";
                if (e)
                {
                    qApp->notify(QApplication::activeWindow(), new QEvent(QEvent::Enter));
                    qApp->notify(QApplication::activeWindow(), new QEvent(QEvent::HoverEnter));
                    qApp->notify(w, new QEvent(QEvent::Enter));
                    qApp->notify(w, new QEvent(QEvent::HoverEnter));
                    qApp->notify(w, new QEvent(QEvent::FocusIn));
                    qApp->notify(w, e);
                    e = new QMouseEvent(QEvent::MouseButtonRelease, w->mapFrom(QApplication::activeWindow(), QPoint(x, y)), QApplication::activeWindow()->mapToGlobal(QPoint(x, y)), Qt::NoButton, Qt::NoButton, m);

                    qApp->notify(w, e);
                    //e = new QMouseEvent(QEvent::MouseButtonPress, w->mapFromParent(QPoint(x, y)), Qt::LeftButton, Qt::LeftButton, m);
                    //qApp->notify(w, e);
//                    QApplication::sendPostedEvents();
                }
            }
        }
        else
            qDebug() << "shit received";
    }
}
