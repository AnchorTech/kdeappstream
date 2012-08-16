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

EventDispather::EventDispather()
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
    qDebug() << "Received data: " << message;
}
