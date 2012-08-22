#include "WidgetsCollection.h"

WidgetsCollection * WidgetsCollection::m_instance = 0;

WidgetsCollection::WidgetsCollection(QObject * parent) :
    QObject(parent),
    m_sem(1)
{
    m_instance = this;
}

WidgetsCollection * WidgetsCollection::instance(QObject * parent)
{
    if (!m_instance)
        new WidgetsCollection(parent);
    return m_instance;
}

bool WidgetsCollection::contains(QWidget * widget)
{
    if (!widget)
        return false;
    m_sem.acquire();
    bool result = m_widgets.contains(widget);
    m_sem.release();
    return result;
}

bool WidgetsCollection::add(QWidget * widget)
{
    if (!widget)
        return false;
    m_sem.acquire();
    if (!m_widgets.contains(widget))
    {
        m_widgets.insert(widget);
        m_sem.release();
        return true;
    }
    m_sem.release();
    return false;
}

bool WidgetsCollection::remove(QWidget * widget)
{
    if (!widget)
        return false;
    m_sem.acquire();
    bool result = m_widgets.remove(widget);
    m_sem.release();
    return result;
}
