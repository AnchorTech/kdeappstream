#include "EventDispatcher.h"
#include "Event.h"
#include "hooking/WidgetsCollection.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QStack>
#include <QDebug>
#include <QApplication>

bool EventDispatcher::_isActivateEvent = false;

EventDispatcher::EventDispatcher(QObject * parent) :
    QObject(parent)
{
}

bool EventDispatcher::eventFilter(QObject * recv, QEvent * e)
{
    Event * event = dynamic_cast<Event*>(e);
    if (!event || !WidgetsCollection::instance()->contains(static_cast<QWidget*>(recv)))
        return false;

    QWidget * widget = dynamic_cast<QWidget*>(recv);
    if (!widget)
        return false;

    QEvent::Type type = event->type();
    if (type == MouseMoveEvent::eventType())
    {
        MouseMoveEvent * mEvent = (MouseMoveEvent*) event;
        this->processEnterEvents(widget, mEvent->pos());
        if (widget->isEnabled())
        {
            QMouseEvent mouseEvent(QEvent::MouseMove, mEvent->pos(), mEvent->globalPos(), mEvent->button(), mEvent->buttons(), mEvent->modifiers());
            if (mEvent->buttons() || widget->hasMouseTracking())
                QCoreApplication::sendEvent(widget, &mouseEvent);

            QStack<QWidget*> hoverObjQueue;
            QStack<QPoint> hoverPosQueue;
            hoverObjQueue.push(widget);
            hoverPosQueue.push(mEvent->pos());
            QWidget * parent = widget->parentWidget();
            while (parent)
            {
                if (parent->testAttribute(Qt::WA_Hover))
                {
                    hoverPosQueue.push(hoverObjQueue.top()->mapToParent(hoverPosQueue.top()));
                    hoverObjQueue.push(parent);
                }
                if (parent->testAttribute(Qt::WA_NoMousePropagation))
                    break;
                parent = parent->parentWidget();
            }

            QPoint pDiff = mEvent->oldPos() - mEvent->pos();

            while (!hoverObjQueue.empty())
            {
                QPoint p = hoverPosQueue.pop();
                QHoverEvent hoverMove(QEvent::HoverMove, p, p + pDiff);
                QCoreApplication::sendEvent(hoverObjQueue.pop(), &hoverMove);
            }
        }
    }
    else if (type == MousePressEvent::eventType())
    {
        MousePressEvent * mEvent = (MousePressEvent*) event;
        if (widget->isEnabled() && mEvent->button())
        {
            if (widget->focusProxy())
                widget = widget->focusProxy();
            if (widget->focusPolicy() & Qt::ClickFocus)
                widget->setFocus(Qt::MouseFocusReason);
            QMouseEvent mouseEvent(QEvent::MouseButtonPress, mEvent->pos(), mEvent->pos(), mEvent->button(), mEvent->buttons(), mEvent->modifiers());
            QCoreApplication::sendEvent(widget, &mouseEvent);
        }
    }
    else if (type == MouseReleaseEvent::eventType())
    {
        MouseReleaseEvent * mEvent = (MouseReleaseEvent*) event;
        if (widget->isEnabled() && mEvent->button())
        {
            QMouseEvent mouseEvent(QEvent::MouseButtonRelease, mEvent->pos(), mEvent->pos(), mEvent->button(), mEvent->buttons(), mEvent->modifiers());
            QCoreApplication::sendEvent(widget, &mouseEvent);
        }
    }
    else if (type == MouseDbClickEvent::eventType())
    {
        MouseDbClickEvent * mEvent = (MouseDbClickEvent*) event;
        if (widget->isEnabled() && mEvent->button())
        {
            QMouseEvent mouseEvent(QEvent::MouseButtonDblClick, mEvent->pos(), mEvent->pos(), mEvent->button(), mEvent->buttons(), mEvent->modifiers());
            QCoreApplication::sendEvent(widget, &mouseEvent);
        }
    }
    else if (type == Activate::eventType())
    {
        _isActivateEvent = true;
        QApplication::setActiveWindow(widget);
        _isActivateEvent = false;
    }

    return true;
}

void EventDispatcher::processEnterEvents(QWidget * mouseGrabber, const QPoint & p)
{
    static QWidget * currentMouseGrabber = 0;
    if (currentMouseGrabber == mouseGrabber)
        return;

    QStack<QWidget*> enterStack;
    QStack<QPoint> enterPoints;
    QStack<QWidget*> leaveStack;
    QStack<QPoint> leavePoints;

    enterStack.push(mouseGrabber);
    enterPoints.push(p);
    QWidget * parentEnter = mouseGrabber->parentWidget();
    while (parentEnter && parentEnter != currentMouseGrabber)
    {
        enterPoints.push(enterStack.top()->mapToParent(p));
        enterStack.push(parentEnter);
        parentEnter = parentEnter->parentWidget();
    }

    if (currentMouseGrabber && !parentEnter)
    {
        QWidget * parentLeave = currentMouseGrabber;
        while (parentLeave && parentLeave != parentEnter)
        {
            leaveStack.push(parentLeave);
            parentLeave = parentLeave->parentWidget();
        }
    }

    QWidget * w = 0;
    QPoint tp(0,0);
    while (enterStack.size() && leaveStack.size() && enterStack.top() == leaveStack.top())
    {
        w = enterStack.pop();
        tp = enterPoints.pop();
        leaveStack.pop();
    }

    QStack<QWidget*>::iterator it = leaveStack.begin();
    while (it != leaveStack.end())
    {
        leavePoints.push_front((*it)->mapFromParent(tp));
        ++it;
    }

    while (leaveStack.size())
    {
        QPoint p = leavePoints.first();
        QWidget * w = leaveStack.first();
        QEvent e(QEvent::Leave);
        QCoreApplication::sendEvent(w, &e);
        if (w->testAttribute(Qt::WA_Hover))
        {
            QHoverEvent e(QEvent::HoverLeave, QPoint(-1, -1), p);
            QCoreApplication::sendEvent(w, &e);
        }
        leavePoints.pop_front();
        leaveStack.pop_front();
    }

    while (enterStack.size())
    {
        QPoint p = enterPoints.pop();
        QWidget * w = enterStack.pop();
        QEvent e(QEvent::Enter);
        QCoreApplication::sendEvent(w, &e);
        if (w->testAttribute(Qt::WA_Hover))
        {
            QHoverEvent e(QEvent::HoverEnter, p, QPoint(-1, -1));
            QCoreApplication::sendEvent(w, &e);
        }
    }

    currentMouseGrabber = mouseGrabber;
}
